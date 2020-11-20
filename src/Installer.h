// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2017 Harald Sitter <sitter@kde.org>

#ifndef INSTALLER_H
#define INSTALLER_H

#include <QObject>
#include <QSet>

class FileResolver;

class Installer : public QObject
{
    Q_OBJECT
public:
    Installer(const QStringList &files);
    void install();

signals:
    void done(int exitCode);

private slots:
    void resolved(FileResolver *resolver);
    void failed(FileResolver *resolver);

private:
    void doInstall();

    QSet<FileResolver *> m_resolvers;
    QSet<QString> m_debugPackageIDs;
    QStringList m_files;
};

#endif // INSTALLER_H
