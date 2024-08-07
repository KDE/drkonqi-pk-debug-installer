// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2020 Harald Sitter <sitter@kde.org>

import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15 as QQC2
import org.kde.kirigami 2.12 as Kirigami
import org.kde.drkonqi.debug.installer.pk 1.0

Kirigami.Page {
    id: page
    title: i18nc("@title", "Debug Symbols Sources")

    QQC2.BusyIndicator {
        id: busy
        anchors.centerIn: parent
        running: DebugRepoEnabler.busy
        visible: running
    }

    ColumnLayout {
        id: column
        anchors.fill: parent
        visible: !busy.visible

        Kirigami.Heading {
            id: label

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter
            horizontalAlignment: Qt.AlignHCenter

            level: 2
            opacity: 0.5

            wrapMode: Text.WordWrap
            text: i18nc("@label/rich", "<p>Additional software sources need to be enabled on this system before debug symbols can be reliably resolved.</p><p>This can be undone in Discover.</p>")
       }

        QQC2.ScrollView {
            id: scrollView

            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            Layout.fillHeight: true

            Kirigami.Theme.colorSet: Kirigami.Theme.View
            Kirigami.Theme.inherit: false
            Component.onCompleted: background.visible = true // only injected and available once completed

            clip: true

            ListView {
                clip: true

                id: view
                interactive: false
                model: DebugRepoEnabler.components

                delegate: Kirigami.SubtitleDelegate {
                    width: ListView.view.width

                    hoverEnabled: ListView.interactive
                    text: modelData.name
                    subtitle: modelData.summary
                }
            }
        }

        QQC2.Button {
            id: installButton
            Layout.alignment: Qt.AlignHCenter
            action: Kirigami.Action {
                icon.name: "install"
                text: i18nc("@button", "Add Missing Software Sources")
                onTriggered: DebugRepoEnabler.install()
            }
        }
    }
}
