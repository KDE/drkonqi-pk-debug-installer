// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2010-2020 Harald Sitter <sitter@kde.org>

#include "DebugRepoEnabler.h"

#include "BuildConfig.h"

void DebugRepoEnabler::run()
{
    connect(this, &DebugRepoEnabler::changed, this, [this] {
        qDebug() << "busy" << m_busy;
    });

    m_busy = true;
    Q_EMIT changed();

    AppStream::Pool pool;
    if (!pool.load()) {
        m_busy = false;
        m_error = QLatin1String("Failed to load appstream pool - ") + pool.lastError();
        Q_EMIT changed();
        return;
    }

    const QString idsString = QString::fromLatin1(DEBUG_REPO_APPSTREAM_IDS);
    if (idsString.isEmpty()) {
        m_busy = false;
        m_installed = true;
        Q_EMIT changed();
        return;
    }

    const QStringList ids = idsString.split(QLatin1Char(';'));
    qDebug() << ids << ids.count();
    Q_ASSERT_X(ids.count() <= 1, Q_FUNC_INFO, "multiple ids are currently not supported - patches welcome");

    QList<AppStream::Component> components;
    for (const auto &id : ids) {
        const auto matchedComponents = pool.componentsById(id);
        components += matchedComponents;

        Q_ASSERT(components.count() == 1); // ensure distros use valid ids
        const auto &component = matchedComponents.at(0);
        qWarning() << component.toString();
        qWarning() << component.packageNames();
        Q_ASSERT(component.kind() == AppStream::Component::KindRepository);
    }

    // TODO: for actual multi-id support the class would need various changes vis a vis tracking what is and isn't
    const auto &component = components.at(0);

    // Quick and dirty mapping of components into blobs so we can pass them to qml.
    m_components.reserve(components.size());
    for (const auto &component : components) {
        m_components << QJsonObject
        {
            {QStringLiteral("id"), component.id()},
            {QStringLiteral("name"), component.name()},
            {QStringLiteral("summary"), component.summary()}
        };
    }
    Q_EMIT changed();

    auto transaction = PackageKit::Daemon::resolve(component.packageNames(), PackageKit::Transaction::FilterArch);
    connect(transaction,
            &PackageKit::Transaction::package,
            this,
            [this](PackageKit::Transaction::Info info, const QString &packageID, const QString &summary) {
                Q_UNUSED(summary);
                m_packageIDs.push_back(packageID);
                m_found = true;
                m_installed = (info == PackageKit::Transaction::InfoInstalled);
                Q_EMIT changed();
            });
    connect(transaction,
            &PackageKit::Transaction::finished,
            this,
            [this](PackageKit::Transaction::Exit status, uint runtime) {
                Q_UNUSED(status);
                Q_UNUSED(runtime);
                m_busy = false;
                Q_EMIT changed();
            });
}

void DebugRepoEnabler::install()
{
    qWarning() << "busy";
    m_busy = true;
    Q_EMIT changed();

    auto transaction = PackageKit::Daemon::installPackages(m_packageIDs);
    connect(transaction,
            &PackageKit::Transaction::errorCode,
            this,
            [this](PackageKit::Transaction::Error error, const QString &details) {
                Q_UNUSED(error);
                m_error = details;
                Q_EMIT changed();
            });
    connect(
        transaction, &PackageKit::Transaction::finished, this, [this](PackageKit::Transaction::Exit status, uint) {
            m_installed = (status == PackageKit::Transaction::ExitSuccess);
            auto transaction = PackageKit::Daemon::refreshCache(false);
            connect(transaction,
                    &PackageKit::Transaction::finished,
                    this,
                    [this](PackageKit::Transaction::Exit status, uint runtime) {
                        Q_UNUSED(status);
                        Q_UNUSED(runtime);
                        m_busy = false;
                        Q_EMIT changed();
                    });
    });
}
