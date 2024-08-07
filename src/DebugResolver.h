// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2017-2020 Harald Sitter <sitter@kde.org>

#pragma once

#include <QObject>
#include <QSet>

#include <PackageKit/Transaction>

#include <memory>

#include "File.h"

class DebugResolver : public QObject
{
    Q_OBJECT
public:
    explicit DebugResolver(std::shared_ptr<File> file, QObject *parent = nullptr);
    void resolve();

Q_SIGNALS:
    void finished();

private Q_SLOTS:
    void transactionFinished();
    void packageFound(PackageKit::Transaction::Info, const QString &packageID, const QString &/* summary */);

private:
    QSet<QObject *> m_transactions;
    const std::shared_ptr<File> m_file;
};
