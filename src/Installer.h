// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2017-2020 Harald Sitter <sitter@kde.org>

#ifndef INSTALLER_H
#define INSTALLER_H

#include <QObject>
#include <QSet>
#include <QList>

#include <memory>

#include "File.h"

class FileResolver;

class Installer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool ready MEMBER m_ready NOTIFY changed)
    Q_PROPERTY(bool installing MEMBER m_installing NOTIFY changed)
    Q_PROPERTY(bool installed MEMBER m_installed NOTIFY changed)
    Q_PROPERTY(QString error MEMBER m_error NOTIFY changed)
    // Trivial helper to access files from QML by decaying the shared pointers to raw pointers.
    Q_PROPERTY(QList<QObject *> files READ fileQObjects CONSTANT)
public:
    explicit Installer(const QStringList &files);
    Q_INVOKABLE void resolve();
    Q_INVOKABLE void install();

    [[nodiscard]] QList<QObject *> fileQObjects() const;

Q_SIGNALS:
    void changed();

private:
    void prepareInstall();

    uint m_pendingResolvers = 0;
    // We are capturing all IDs outside the files as well so we can uniq! them
    // and ensure we even have anything to install before the user clicks install.
    QStringList m_debugPackageIDs;
    std::vector<std::shared_ptr<File>> m_files;
    bool m_ready = false;
    bool m_installing = false;
    bool m_installed = false;
    QString m_error;
};

#endif // INSTALLER_H
