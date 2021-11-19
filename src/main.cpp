// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2010-2021 Harald Sitter <sitter@kde.org>

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QCommandLineParser>
#include <QUrl>
#include <QQmlContext>
#include <QIcon>

#include <KAboutData>
#include <KLocalizedString>
#include <KDeclarative/KDeclarative>
#include <KLocalizedContext>

#include "Debug.h"
#include "Logger.h"
#include "Installer.h"
#include "BuildConfig.h"
#include "DebugRepoEnabler.h"

int main(int argc, char *argv[])
{
    Logger::instance()->install();

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    // Desktop style requires QApplication not QGuiApplication.
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("tools-report-bug")));
    app.setDesktopFileName(QStringLiteral("org.kde.drkonqi-pk-debug-installer"));

    KAboutData aboutData(QStringLiteral("drkonqi-pk-debug-installer"),
                         i18nc("@title", "Debug Symbols Helper"),
                         QString::fromLatin1(VERSION),
                         i18n("A debug package installer using PackageKit"),
                         KAboutLicense::LicenseKey::GPL,
                         i18n("(C) 2010-2020 Harald Sitter"));

    aboutData.addAuthor(i18n("Harald Sitter"),
                        QStringLiteral(),
                        QStringLiteral("sitter@kde.org"));

    QCommandLineParser parser;
    parser.addPositionalArgument(QStringLiteral("FILES"),
                                 i18nc("command line description", "Files to find debug packages for"));
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    auto files = parser.positionalArguments();
    if (files.isEmpty()) {
        return 0;
    }
    qCDebug(INSTALLER) << "files:" << files;

    DebugRepoEnabler repoEnabler;
    qmlRegisterSingletonInstance("org.kde.drkonqi.debug.installer.pk", 1, 0, "DebugRepoEnabler", &repoEnabler);
    Installer installer(files);
    qmlRegisterSingletonInstance("org.kde.drkonqi.debug.installer.pk", 1, 0, "Installer", &installer);
    qmlRegisterSingletonInstance("org.kde.drkonqi.debug.installer.pk", 1, 0, "Logger", Logger::instance());

    KLocalizedContext i18nContext;
    i18nContext.setTranslationDomain(QStringLiteral(TRANSLATION_DOMAIN));

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextObject(&i18nContext);

    KDeclarative::KDeclarative::setupEngine(&engine);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
            QCoreApplication::exit(-1);
        }
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
