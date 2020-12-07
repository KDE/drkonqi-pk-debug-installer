// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2020 Harald Sitter <sitter@kde.org>

import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15 as QQC2
import org.kde.kirigami 2.12 as Kirigami
import org.kde.drkonqi.debug.installer.pk 1.0

Kirigami.ApplicationWindow {
    id: root

    title: i18nc("@title:window", "Debug Symbols Installer")
    minimumWidth: Kirigami.Units.gridUnit * 22
    minimumHeight: Kirigami.Units.gridUnit * 22

    // Window instances aren't Items so we need a helper to do clean state management.
    // Errors are currently a one-way road. We'll not let the user recover from there to save some time developing
    // comprehensive state resets in the c++ code.
    // The user will ultimately still be able to repeat the install via drkonqi.
    Item {
        state: {
            if (DebugRepoEnabler.error !== "") {
                return "error"
            }
            if (Installer.error !== "") {
                return "error"
            }
            if (!DebugRepoEnabler.installed) {
                return "debug-repo"
            }
            "normal"
        }

        states: [
            State {
                name: "debug-repo"
                PropertyChanges { target: pageStack; initialPage: "qrc:/DebugRepoPage.qml" }
            },
            State {
                name: "normal"
                PropertyChanges { target: pageStack; initialPage: "qrc:/MainPage.qml" }
            },
            State {
                name: "error"
                PropertyChanges { target: pageStack; initialPage: "qrc:/ErrorPage.qml" }
            }
        ]
    }

    // TODO: an argument could be made that a global footer would be nicer, it's also trickier as the content is
    //  depending on the "main" page. i.e. we have different dialogboxes depending on whether
    //  we are on the initial page or the error page. both of which are "main" pages

    // footer: {
    //     console.log("!!!!!!!!!!!!!!!!!!!")
    //     console.log(pageStack.items[0])
    //     console.log(pageStack.items[0].title)
    //     console.log(pageStack.items[0].globalFooter)
    //     pageStack.items[0].globalFooter === undefined ? null : pageStack.items[0].globalFooter

    // }

    Component.onCompleted: DebugRepoEnabler.run()
}
