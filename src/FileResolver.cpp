// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2017 Harald Sitter <sitter@kde.org>

#include "FileResolver.h"

#include <QDebug>
#include <QProcess>

#include <PackageKit/Daemon>

#include "DebugResolver.h"

class Package
{
public:
    Package(const QString &id)
        : m_id(id)
    {}

    QString version() const
    {
        return PackageKit::Daemon::packageVersion(m_id);
    }

    QString arch() const
    {
        return PackageKit::Daemon::packageArch(m_id);
    }

protected:
    QString m_id;
};

class DebugPackage : public Package
{
public:
    using Package::Package;

    bool isCompatibleWith(const QString &id)
    {
        return isCompatibleWith(Package(id));
    }

    bool isCompatibleWith(const Package &package)
    {
        return version() == package.version() && arch() == package.arch();
    }
};

void FileResolver::resolve(const QString &file)
{
    auto *transaction =
            PackageKit::Daemon::searchFiles(file, PackageKit::Transaction::FilterInstalled);
    qDebug() << transaction;
    connect(transaction, &PackageKit::Transaction::package, this, &FileResolver::packageFound);
    connect(transaction, &PackageKit::Transaction::finished, this, [&]() {
        if (m_packageID.isEmpty()) {
            emit failed(this);
        }
    });
}

QString FileResolver::packageID() const
{
    return m_packageID;
}

QStringList FileResolver::debugCandidateIDs() const
{
    return m_debugCandidateIDs;
}

// PackageKit does not expose the source of a package. As we need to support source-dbg for legacy
// reasons we'll dpkg-query the source here. This is a lock-less operation so it should be fine.
// Still shitty to have to go through dpkg-query and not simply defer to packagekit -.-
static QString dpkgSourceName(const QString &packageID)
{
    auto package = PackageKit::Daemon::packageName(packageID);

    const auto packageArch = PackageKit::Daemon::packageArch(packageID);
    if (!packageArch.isEmpty()) {
        package = package + ":" + packageArch;
    }

    QProcess proc;
    proc.start(QStringLiteral("dpkg-query"),
               QStringList() << QStringLiteral("-f=${Source}\n") << QStringLiteral("-W") << package);
    qDebug() << proc.arguments();
    proc.waitForFinished();

    return QString::fromLatin1(proc.readLine()).trimmed();
}

void FileResolver::packageFound(PackageKit::Transaction::Info, const QString &packageID, const QString &)
{
    qDebug() << this << "found" << packageID;
    m_packageID = packageID;

    const auto packageName = PackageKit::Daemon::packageName(packageID);

    QStringList debugCandidates;
    debugCandidates << packageName + "-dbgsym"
                    << packageName + "-dbg";

    const auto sourceName = dpkgSourceName(packageID);
    if (!sourceName.isEmpty()) {
        // -dbg would be a manaual created one, so it should take preference here.
        debugCandidates << sourceName + "-dbg"
                        << sourceName + "-dbgsym";;
    }

    qDebug() << "candidates" << debugCandidates;

    m_debugResolver = new DebugResolver(this);
    connect(m_debugResolver, &DebugResolver::finished, this, &FileResolver::debugResolverFinished);
    m_debugResolver->resolve(debugCandidates);
}

void FileResolver::debugResolverFinished()
{
    auto resolver = m_debugResolver;
    m_debugResolver->deleteLater(); // Only happens once we return.
    m_debugResolver = nullptr;
    auto candidateIDs = resolver->candidateIDs();
    for (auto it = candidateIDs.begin(); it != candidateIDs.end();) {
        if (!DebugPackage(*it).isCompatibleWith(m_packageID)) {
            qDebug() << "  -- dropping incompatible candidate" << m_packageID;
            it = candidateIDs.erase(it);
        } else {
            ++it;
        }
    }
    qDebug() << this << "++ packages for" << m_packageID << candidateIDs;
    m_debugCandidateIDs = candidateIDs;
    emit resolved(this);
}
