// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2017-2020 Harald Sitter <sitter@kde.org>

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
    void packageFound(PackageKit::Transaction::Info, const QString &packageID, const QString &/* summary */);

private:
    QStringList m_possibleCandidates;
    QStringList m_candidates;
    QSet<QObject *> m_transactions;
};

#endif // DEBUGRESOLVER_H
