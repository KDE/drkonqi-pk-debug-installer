// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2020 Harald Sitter <sitter@kde.org>

#include "Logger.h"

#include "Debug.h"

Logger *Logger::instance()
{
    static Logger logger;
    return &logger;
}

void Logger::install()
{
    previousHandler = qInstallMessageHandler(&Logger::handler);
}

Q_INVOKABLE QStringList Logger::log()
{
    return data;
}

void Logger::handler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Record our category and the default (default is not categorized output)
    if (strcmp(context.category, INSTALLER().categoryName()) == 0 ||
        strcmp(context.category, "default") == 0) {
        instance()->data << QStringLiteral("[%1] %2\n").arg(QString::fromLatin1(context.function), msg);
    }
    // Forward all calls to the default handler.
    if (auto previous = instance()->previousHandler; previous) {
        previous(type, context, msg);
    }
}
