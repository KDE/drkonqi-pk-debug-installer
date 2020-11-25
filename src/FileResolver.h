// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2017-2020 Harald Sitter <sitter@kde.org>

#ifndef FILERESOLVER_H
#define FILERESOLVER_H

#include <QObject>

#include <PackageKit/Transaction>
#include <memory>

#include "File.h"

class File;

class FileResolver : public QObject
{
    Q_OBJECT
public:
    explicit FileResolver(std::shared_ptr<File> file, QObject *parent = nullptr);

    void resolve();

signals:
    void finished();

private slots:
    void packageFound(PackageKit::Transaction::Info, const QString &packageID, const QString &/* summary */);
    void debugResolverFinished();

private:
    const std::shared_ptr<File> m_file;
};

#endif // FILERESOLVER_H
