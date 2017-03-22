/*
    Copyright © 2017 Harald Sitter <sitter@kde.org>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License or (at your option) version 3 or any later version
    accepted by the membership of KDE e.V. (or its successor approved
    by the membership of KDE e.V.), which shall act as a proxy
    defined in Section 14 of version 3 of the license.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "FileResolver.h"

#include <QDebug>

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
}

QString FileResolver::packageID() const
{
    return m_packageID;
}

QStringList FileResolver::debugCandidateIDs() const
{
    return m_debugCandidateIDs;
}

void FileResolver::packageFound(PackageKit::Transaction::Info, const QString &packageID, const QString &)
{
    qDebug() << this << "found" << packageID;
    m_packageID = packageID;

    const auto packageName = PackageKit::Daemon::packageName(packageID);
    QStringList debugCandidates;
    debugCandidates << packageName + "-dbgsym"
                    << packageName + "-dbg";
    qDebug() << "candidates" << debugCandidates;

    m_debugResolver = new DebugResolver(this);
    connect(m_debugResolver, &DebugResolver::finished, this, &FileResolver::debugResolverFinished);
    m_debugResolver->resolve(debugCandidates);
}

void FileResolver::debugResolverFinished()
{
    QScopedPointer<DebugResolver> DebugResolver(m_debugResolver);
    m_debugResolver = nullptr;
    auto candidateIDs = DebugResolver->candidateIDs();
    for (auto it = candidateIDs.begin(); it != candidateIDs.end();) {
        if (!DebugPackage(*it).isCompatibleWith(m_packageID)) {
            it = candidateIDs.erase(it);
        } else {
            ++it;
        }
    }
    qDebug() << this << "++ packages for" << m_packageID << candidateIDs;
    m_debugCandidateIDs = candidateIDs;
    emit resolved(this);
}
