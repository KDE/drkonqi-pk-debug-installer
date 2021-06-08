// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2020 Harald Sitter <sitter@kde.org>

#pragma once

#include <QObject>

#include <memory>

#include "File.h"

class DiagnosticResolver : public QObject
{
    Q_OBJECT
public:
    explicit DiagnosticResolver(std::shared_ptr<File> file, QObject *parent = nullptr);
    void resolve();

    [[nodiscard]] QString data() const;

signals:
    void finished();

private:
    const std::shared_ptr<File> m_file;
    QString m_data;
};
