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
