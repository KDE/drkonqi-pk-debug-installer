// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2010-2017 Harald Sitter <sitter@kde.org>

#include <QApplication>
#include <QDebug>
#include <QCommandLineParser>
#include <QUrl>

#include <KAboutData>
#include <KLocalizedString>

#include "Installer.h"
#include "Version.h"

// TODO: maybe inject ubuntu dbgsym sources.list so a greater pool of packges is available
//struct Repo
//{
//    Repo(const QString &id, bool enabled_)
//        : enabled(enabled_)
//    {
//        file = id.section(':', 0, 0);
//        QStringList parts = id.section(':', 1).split(' ');
//        Q_ASSERT(parts.length() >= 3 /* type uri pocket */);
//        type = parts.takeFirst();
//        url = QUrl(parts.takeFirst());
//        pocket = parts.takeFirst();
//        qualifiers = parts;
//    }
//
//    QString file;
//    QString type;
//    QUrl url;
//    QString pocket;
//    QStringList qualifiers;
//    bool enabled;
//};

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    KAboutData aboutData("drkonqi-pk-debug-installer",
                         i18n("Debug package installer"),
                         version,
                         i18n("A debug package installer using PackageKit"),
                         KAboutLicense::LicenseKey::GPL,
                         i18n("(C) 2010-2017 Harald Sitter"));

    aboutData.addAuthor(i18n("Harald Sitter"),
                        QStringLiteral(),
                        QStringLiteral("sitter@kde.org"));

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("FILES",
                                 i18n("Files to find debug packages for"));
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    auto files = parser.positionalArguments();
    if (files.isEmpty()) {
        return 0;
    }
    Installer installer(files);
    QObject::connect(&installer, &Installer::done, [&](int exitCode) { qApp->exit(exitCode); });
    installer.install();

    return app.exec();
}

#include "main.moc"
