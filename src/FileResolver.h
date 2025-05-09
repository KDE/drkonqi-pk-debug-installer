// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2017-2021 Harald Sitter <sitter@kde.org>

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
    void resolve(const QString &path);

Q_SIGNALS:
    void finished();

private Q_SLOTS:
    void packageFound(PackageKit::Transaction::Info, const QString &packageID, const QString &/* summary */);
    void debugResolverFinished();

private:
    const std::shared_ptr<File> m_file;
    bool m_triedUsrMerge = false;
};

#endif // FILERESOLVER_H
