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

#ifndef DEBUGRESOLVER_H
#define DEBUGRESOLVER_H

#include <QObject>
#include <QSet>

#include <PackageKit/Transaction>

class DebugResolver : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    void resolve(const QStringList &candidates);

    QStringList candidateIDs() const;

signals:
    void finished();

private slots:
    void transactionFinished();
    void packageFound(PackageKit::Transaction::Info i, const QString &packageID, const QString &/* summary */);

private:
    QStringList m_candidates;
    QSet<QObject *> m_transactions;
};

#endif // DEBUGRESOLVER_H
