// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2017 Harald Sitter <sitter@kde.org>

#ifndef FILERESOLVER_H
#define FILERESOLVER_H

#include <QObject>
#include <QStringList>

#include <PackageKit/Transaction>

class DebugResolver;

class FileResolver : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

    void resolve(const QString &file);

    QString packageID() const;
    QStringList debugCandidateIDs() const;

signals:
    void resolved(FileResolver *);
    void failed(FileResolver *);

private slots:
    void packageFound(PackageKit::Transaction::Info, const QString &packageID, const QString &/* summary */);
    void debugResolverFinished();

private:
    QString m_packageID;
    QStringList m_debugCandidateIDs;
    DebugResolver *m_debugResolver = nullptr;
};

#endif // FILERESOLVER_H
