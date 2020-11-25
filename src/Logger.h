// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2020 Harald Sitter <sitter@kde.org>

#pragma once

#include <QObject>

// Installs a qt message handler to capture all qDebugs inside the process
// so we can display them to the user on error in hopes of the user telling us
// and the log making it easier to figure out what went wrong.
class Logger : public QObject
{
    Q_OBJECT
public:
    // Singleton instance
    static Logger *instance();
    // Installs the qMessageHandler
    void install();
    // Dump the captured log
    Q_INVOKABLE QStringList log();

private:
    static void handler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    QtMessageHandler previousHandler = nullptr;
    QStringList data;
};
