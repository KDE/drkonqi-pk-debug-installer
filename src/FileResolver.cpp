// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2017-2020 Harald Sitter <sitter@kde.org>

#include "FileResolver.h"

#include <QDebug>
#include <QProcess>

#include <PackageKit/Daemon>
#include <utility>

#include "Debug.h"
#include "DebugResolver.h"

FileResolver::FileResolver(std::shared_ptr<File> file, QObject *parent)
    : QObject(parent)
    , m_file(std::move(file))
{
}

void FileResolver::resolve()
{
    auto *transaction = PackageKit::Daemon::searchFiles(m_file->path(), PackageKit::Transaction::FilterInstalled);
    connect(transaction, &PackageKit::Transaction::package, this, &FileResolver::packageFound);
    connect(transaction, &PackageKit::Transaction::finished, this, [this]() {
        sender()->deleteLater();
        if (m_file->packageID().isEmpty()) {
            // If we failed to resolve the effective package we'll still want
            // to get cleaned up, naturally.
            // This is kinda error handled in the Installer as well as the QML.
            // If we failed to resolve any debug packages that is a fatal error otherwise we'll install
            // the ones that managed to resolve by mark the broken packages in the UI as being broken.
            m_file->setResolved();
            Q_EMIT finished();
        }
    });
}

void FileResolver::packageFound(PackageKit::Transaction::Info, const QString &packageID, const QString &)
{
    qCDebug(INSTALLER) << this << "found" << packageID;

    m_file->setPackageID(packageID);

    auto package = m_file->package();
    const auto packageArch = PackageKit::Daemon::packageArch(packageID);
    if (!packageArch.isEmpty()) {
        package = package + QLatin1Char(':') + packageArch;
    }

    // Packagekit has no notion of source packages, we'll need to resolve it manually.
    // This is legacy compatibility for the most part. These days debug symbols are generally generated for each
    // binary. Originally manually created dbg packages were per source though, I suppose they can still appear.
    auto proc = new QProcess(this);
    connect(proc, &QProcess::finished, this, [this, proc](int exitCode, QProcess::ExitStatus) {
        proc->deleteLater();

        if (exitCode == 0) {
            m_file->setSourcePackage(QString::fromLatin1(proc->readLine()).trimmed());
        }
        // else isn't a fatal problem as most (or even all) modern packages have -dbgsym packges
        // instead of source-derived -dbg packages.

        auto resolver = new DebugResolver(m_file, this);
        connect(resolver, &DebugResolver::finished, this, &FileResolver::debugResolverFinished);
        resolver->resolve();
    });

    proc->start(QStringLiteral("dpkg-query"),
               QStringList() << QStringLiteral("-f=${source:Package}\n") << QStringLiteral("-W") << package);
}

void FileResolver::debugResolverFinished()
{
    sender()->deleteLater();
    qCDebug(INSTALLER) << this << "++ packages for" << m_file->packageID() << m_file->debugPackageID();
    m_file->setResolved();
    Q_EMIT finished();
}
