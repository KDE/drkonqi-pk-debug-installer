/*
    Copyright © 2017 Harald Sitter <sitter@kde.org>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License or (at your option) version 3 or any later version
    accepted by the membership of KDE e.V. (or its successor approved
    by the membership of KDE e.V.), which shall act as a proxy
    defined in Section 14 of version 3 of the license.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
    for (const auto &file : m_files) {
        auto r = new FileResolver(this);
        m_resolvers << r;
        connect(r, &FileResolver::resolved, this, &Installer::resolved);
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

void Installer::doInstall()
{
    if (m_debugPackageIDs.isEmpty()) {
        emit done(0);
        return;
    }
    qDebug() << "DO INSTALL" << m_debugPackageIDs;
    auto transaction = PackageKit::Daemon::installPackages(m_debugPackageIDs.toList());
    QObject::connect(transaction, &PackageKit::Transaction::finished,
                     [&](PackageKit::Transaction::Exit status, uint) {
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
