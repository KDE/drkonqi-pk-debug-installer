// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2017-2020 Harald Sitter <sitter@kde.org>

#pragma once

#include <QObject>

class File : public QObject
{
    Q_OBJECT
    // The path of the file that is meant to get debug symbols (incoming arg).
    Q_PROPERTY(QString path READ path CONSTANT)
    // The package the file belongs to. This is the packagekit id!
    Q_PROPERTY(QString packageID READ packageID NOTIFY changed)
    // The package the file belongs to.
    Q_PROPERTY(QString package READ package NOTIFY changed)
    // The source package that created the package.
    Q_PROPERTY(QString sourcePackage READ sourcePackage NOTIFY changed)
    // The candidate names for debug packages. These are likely name
    // combinations of packages that may provide debug packages in order of preference.
    // These are packagekit ids not actual package names!
    Q_PROPERTY(QString debugPackageID READ debugPackageID NOTIFY changed)
    Q_PROPERTY(bool debugPackageInstalled READ isDebugPackageInstalled NOTIFY changed)
    // When completely resolved
    Q_PROPERTY(bool resolved READ isResolved NOTIFY resolved)
public:
    explicit File(QString path, QObject *parent = nullptr);

    // All possibly name permutations of the debug package associated with this
    // file.
    [[nodiscard]] QStringList potentialDebugPackageCandidateNames() const;

    [[nodiscard]] QString path() const;

    [[nodiscard]] QString package() const;
    [[nodiscard]] QString packageID() const;
    void setPackageID(const QString &packageID);

    [[nodiscard]] QString sourcePackage() const;
    void setSourcePackage(const QString &sourcePackageName);

    [[nodiscard]] bool isDebugPackageInstalled() const;
    void setDebugPackageInstalled();

    [[nodiscard]] QString debugPackageID() const;
    void setDebugPackageIDAndStatus(const QString &debugPackageID, bool installed);

    [[nodiscard]] bool isResolved() const;
    void setResolved();

Q_SIGNALS:
    void changed();
    void resolved();

private:
    const QString m_path;
    QString m_packageID;
    QString m_sourcePackage;
    QString m_debugPackageID;
    bool m_debugPackageInstalled = false;

    bool m_resolved = false;
};
