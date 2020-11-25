// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2020 Harald Sitter <sitter@kde.org>

#pragma once

#include <QObject>

class Package : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id MEMBER id CONSTANT)
    Q_PROPERTY(QString name MEMBER name CONSTANT)
    Q_PROPERTY(QString version MEMBER version CONSTANT)
    Q_PROPERTY(QString arch MEMBER arch CONSTANT)
public:
    explicit Package(const QString &id, QObject *parent = nullptr);

private:
    const QString id;
    const QString name;
    const QString version;
    const QString arch;

    friend class DebugPackage;
};
