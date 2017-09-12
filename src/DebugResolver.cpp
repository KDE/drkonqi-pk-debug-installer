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

#include "DebugResolver.h"

#include <QDebug>

#include <PackageKit/Daemon>

void DebugResolver::resolve(const QStringList &candidates)
{
    for (const auto &candidate : candidates) {
        auto *transaction =
                PackageKit::Daemon::searchNames(candidate, PackageKit::Transaction::FilterNotInstalled);
        m_transactions << transaction;
        connect(transaction, &PackageKit::Transaction::finished,
                this, &DebugResolver::transactionFinished);
        connect(transaction, &PackageKit::Transaction::package,
                this, &DebugResolver::packageFound);
    }
}

QStringList DebugResolver::candidateIDs() const
{
    return m_candidates;
}

void DebugResolver::transactionFinished()
{
    qDebug();
    m_transactions.remove(sender());
    if (m_transactions.isEmpty()) {
        emit finished();
    }
}

void DebugResolver::packageFound(PackageKit::Transaction::Info, const QString &packageID, const QString &)
{
    qDebug() << this << "dbgfound" << packageID << m_candidates;
    if (!m_candidates.contains(packageID)) {
        m_candidates << packageID;
    }
    qDebug() << this << m_candidates.size();
}
