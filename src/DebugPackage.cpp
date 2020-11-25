// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2017-2020 Harald Sitter <sitter@kde.org>

#include "DebugPackage.h"

bool DebugPackage::isCompatibleWith(const QString &id)
{
    return isCompatibleWith(Package(id));
}

bool DebugPackage::isCompatibleWith(const Package &package)
{
    return version == package.version && arch == package.arch;
}
