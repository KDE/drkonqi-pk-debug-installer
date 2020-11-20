// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2017-2020 Harald Sitter <sitter@kde.org>

#include "DebugResolver.h"

#include <QDebug>

#include <PackageKit/Daemon>

void DebugResolver::resolve(const QStringList &candidates)
{
    m_possibleCandidates = candidates;
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
    // searchNames matches the search string anywhere in the name so we need
    // to further filter the list to exact matches.
    // Otherwise foo-dbgsym will have libfoo-dbgsym as candidate even though
    // it may be entirely unrelated to the crash.
    const auto packageName = PackageKit::Daemon::packageName(packageID);
    if (!m_possibleCandidates.contains(packageName)) {
        return;
    }
    qDebug() << this << "dbgfound" << packageID << m_candidates;
    if (!m_candidates.contains(packageID)) {
        m_candidates << packageID;
    }
    qDebug() << this << m_candidates.size();
}
