// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2017-2020 Harald Sitter <sitter@kde.org>

#include "DebugResolver.h"

#include <QDebug>

#include <PackageKit/Daemon>

#include "Debug.h"
#include "DebugPackage.h"

DebugResolver::DebugResolver(std::shared_ptr<File> file, QObject *parent)
    : QObject(parent)
    , m_file(std::move(file))
{
}

void DebugResolver::resolve()
{
    const QStringList candidates = m_file->potentialDebugPackageCandidateNames();
    qCDebug(INSTALLER) << "candidates" << candidates;
    for (const auto &candidate : candidates) {
        #warning this is incredibly slow because aptcc is sequential we should instead grab all packages in a single transaction and then filter client side
        // NB: https://github.com/hughsie/PackageKit/issues/441
        auto *transaction = PackageKit::Daemon::searchNames(candidate);
        m_transactions << transaction;
        connect(transaction, &PackageKit::Transaction::finished,
                this, &DebugResolver::transactionFinished);
        connect(transaction, &PackageKit::Transaction::package,
                this, &DebugResolver::packageFound);
    }
}

void DebugResolver::transactionFinished()
{
    m_transactions.remove(sender());
    if (m_transactions.isEmpty()) {
        Q_EMIT finished();
    }
}

void DebugResolver::packageFound(PackageKit::Transaction::Info info, const QString &packageID, const QString &)
{
    // searchNames matches the search string anywhere in the name so we need
    // to further filter the list to exact matches.
    // Otherwise foo-dbgsym will have libfoo-dbgsym as candidate even though
    // it may be entirely unrelated to the crash.
    const auto packageName = PackageKit::Daemon::packageName(packageID);
    if (!m_file->potentialDebugPackageCandidateNames().contains(packageName)) {
        return;
    }
    if (!DebugPackage(packageID).isCompatibleWith(m_file->packageID())) {
        qCDebug(INSTALLER) << "  -- ignoring incompatible candidate" << packageID << m_file->packageID();
        return;
    }
    m_file->setDebugPackageIDAndStatus(packageID, info == PackageKit::Transaction::InfoInstalled);
}
