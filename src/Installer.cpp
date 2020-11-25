// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2017-2020 Harald Sitter <sitter@kde.org>

#include "Installer.h"

#include <QApplication>
#include <QDebug>

#include <PackageKit/Daemon>
#include <KLocalizedString>

#include "Debug.h"
#include "FileResolver.h"

Installer::Installer(const QStringList &files)
{
    for (const auto &file : files) {
        m_files.push_back(std::make_shared<File>(file));
    }
}

QList<QObject *> Installer::fileQObjects() const
{
    QList<QObject *> list;
    list.reserve(m_files.size());
    for (const auto &file : std::as_const(m_files)) {
        list << file.get();
    }
    return list;
}

void Installer::resolve()
{
    for (const auto &file : std::as_const(m_files)) {
        auto resolver = new FileResolver(file, this);
        ++m_pendingResolvers;
        connect(resolver, &FileResolver::finished, this, [this, file] {
            sender()->deleteLater();
            qCDebug(INSTALLER) << "RESOLVED" << file->packageID() << file->debugPackageID() << m_pendingResolvers;
            if (!file->debugPackageID().isEmpty() && !file->isDebugPackageInstalled()) {
                m_debugPackageIDs << file->debugPackageID();
            }
            if (--m_pendingResolvers <= 0)  {
                prepareInstall();
            }
        });
        resolver->resolve();
    }
}

void Installer::prepareInstall()
{
    std::sort(m_debugPackageIDs.begin(), m_debugPackageIDs.end());
    auto last = std::unique(m_debugPackageIDs.begin(), m_debugPackageIDs.end());
    m_debugPackageIDs.erase(last, m_debugPackageIDs.end());

    if (m_debugPackageIDs.isEmpty()) {
        m_error = i18nc("@label", "Failed to find any suitable debug packages or they are all already installed.");
        Q_EMIT changed();
    }

    m_ready = true;
    Q_EMIT changed();
}

void Installer::install() {
    m_installing = true;
    Q_EMIT changed();

    qDebug() << "DO INSTALL" << m_debugPackageIDs;

    auto transaction = PackageKit::Daemon::installPackages(m_debugPackageIDs);
    connect(transaction,
            &PackageKit::Transaction::errorCode,
            this,
            [&](PackageKit::Transaction::Error, const QString &details) {
                m_error = details;
                Q_EMIT changed();
            });
    connect(transaction, &PackageKit::Transaction::finished, this, [&](PackageKit::Transaction::Exit status, uint) {
        m_installing = false;
        m_installed = true;
        Q_EMIT changed();

        switch (status) {
        case PackageKit::Transaction::ExitSuccess:
            for (const auto &id : std::as_const(m_debugPackageIDs)) {
                for (const auto &file : std::as_const(m_files)) {
                    if (id == file->debugPackageID()) {
                        file->setDebugPackageInstalled();
                    }
                }
            }
            return;
        default:
            qCDebug(INSTALLER) << "unexpected exit status" << status;
            return;
        }
    });
}
