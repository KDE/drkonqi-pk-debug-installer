// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2020 Harald Sitter <sitter@kde.org>

#pragma once

#include "Package.h"

class DebugPackage : public Package
{
public:
    using Package::Package;

    bool isCompatibleWith(const QString &id);
    bool isCompatibleWith(const Package &package);
};
