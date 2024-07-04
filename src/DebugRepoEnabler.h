// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2010-2020 Harald Sitter <sitter@kde.org>

#pragma once

#include <AppStreamQt5/pool.h>
#include <PackageKit/Daemon>
#include <QJsonObject>
#include <QVector>

class DebugRepoEnabler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool busy MEMBER m_busy NOTIFY changed)
    Q_PROPERTY(bool found MEMBER m_found NOTIFY changed)
    Q_PROPERTY(bool installed MEMBER m_installed NOTIFY changed)
    Q_PROPERTY(QString error MEMBER m_error NOTIFY changed)
    Q_PROPERTY(QVector<QJsonObject> components MEMBER m_components NOTIFY changed)
public:
    using QObject::QObject;

    Q_INVOKABLE void run();
    Q_INVOKABLE void install();

Q_SIGNALS:
    void changed();

private:
    bool m_busy = true;
    bool m_found = false;
    bool m_installed = false;
    QString m_error;
    QStringList m_packageIDs; // list so we can feed it to packagekit for install easily
    QVector<QJsonObject> m_components; // QVector for qml
};
