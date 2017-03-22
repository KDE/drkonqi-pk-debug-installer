/*
    Copyright © 2010-2017 Harald Sitter <sitter@kde.org>

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
