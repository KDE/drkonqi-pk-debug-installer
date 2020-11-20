// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2017 Harald Sitter <sitter@kde.org>

#include "Installer.h"

#include <QApplication>
#include <QDebug>

#include <PackageKit/Daemon>

#include "FileResolver.h"

Installer::Installer(const QStringList &files)
    : m_files(files)
{
}

void Installer::install()
{
    for (const auto &file : qAsConst(m_files)) {
        auto r = new FileResolver(this);
        m_resolvers << r;
        connect(r, &FileResolver::resolved, this, &Installer::resolved);
        connect(r, &FileResolver::failed, this, &Installer::failed);
        r->resolve(file);
    }
}

void Installer::resolved(FileResolver *resolver)
{
    qDebug() << resolver->packageID() << resolver->debugCandidateIDs() << m_resolvers << resolver;
    m_debugPackageIDs += resolver->debugCandidateIDs().toSet();
    m_resolvers.remove(resolver);
    if (m_resolvers.isEmpty()) {
        doInstall();
    }
    resolver->deleteLater();
}

void Installer::failed(FileResolver *resolver)
{
    resolver->deleteLater();
    emit done(2); // 2 = symbols not found
}

void Installer::doInstall()
{
    if (m_debugPackageIDs.isEmpty()) {
        emit done(0);
        return;
    }
    qDebug() << "DO INSTALL" << m_debugPackageIDs;
    auto transaction = PackageKit::Daemon::installPackages(m_debugPackageIDs.toList());
    QObject::connect(transaction, &PackageKit::Transaction::finished,
                     this, [&](PackageKit::Transaction::Exit status, uint) {
        switch (status) {
        case PackageKit::Transaction::ExitSuccess:
            emit done(0);
            return;
        default:
            qDebug() << status;
            emit done(1);
            return;
        }
    });
}
