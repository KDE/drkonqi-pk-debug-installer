// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2017-2020 Harald Sitter <sitter@kde.org>

#include "File.h"

#include <PackageKit/Daemon>

File::File(QString path, QObject *parent)
    : QObject(parent)
    , m_path(std::move(path))
{
}

QStringList File::potentialDebugPackageCandidateNames() const
{
    auto pkgName = package();
    QStringList candidates{pkgName + QStringLiteral("-dbgsym"), pkgName + QStringLiteral("-dbg")};
    if (!m_sourcePackage.isEmpty()) {
        // -dbg would be a manual created one, so it should take preference here.
        candidates << m_sourcePackage + QStringLiteral("-dbg")
                    << m_sourcePackage + QStringLiteral("-dbgsym");
    }
    return candidates;
}

QString File::path() const
{
    return m_path;
}

QString File::packageID() const
{
    return m_packageID;
}

void File::setPackageID(const QString &packageID)
{
    m_packageID = packageID;
    Q_EMIT changed();
}

QString File::package() const
{
    // Trivial string splitting internally, no use caching.
    return PackageKit::Daemon::packageName(packageID());
}

QString File::sourcePackage() const
{
    return m_sourcePackage;
}

void File::setSourcePackage(const QString &sourcePackageName)
{
    m_sourcePackage = sourcePackageName;
    Q_EMIT changed();
}

void File::setDebugPackageIDAndStatus(const QString &debugPackageID, bool installed)
{
    m_debugPackageID = debugPackageID;
    m_debugPackageInstalled = installed;
    Q_EMIT changed();
}

QString File::debugPackageID() const
{
    return m_debugPackageID;
}

bool File::isDebugPackageInstalled() const
{
    return m_debugPackageInstalled;
}

void File::setDebugPackageInstalled()
{
    m_debugPackageInstalled = true;
    Q_EMIT changed();
}

void File::setResolved()
{
    m_resolved = true;
    Q_EMIT resolved();
}

bool File::isResolved() const
{
    return m_resolved;
}
