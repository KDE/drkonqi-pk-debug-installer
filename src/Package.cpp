// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2020 Harald Sitter <sitter@kde.org>

#include "Package.h"

#include <PackageKit/Daemon>

Package::Package(const QString &id, QObject *parent)
    : QObject(parent)
    , id(id)
    , name(PackageKit::Daemon::packageName(id))
    , version(PackageKit::Daemon::packageVersion(id))
    , arch(PackageKit::Daemon::packageArch(id))
{
}
