// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2020 Harald Sitter <sitter@kde.org>

import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15 as QQC2
import org.kde.kirigami 2.4 as Kirigami
import org.kde.drkonqi.debug.installer.pk 1.0

Kirigami.Page {
    id: page
    title: i18nc("@title", "Error")

    footer: QQC2.DialogButtonBox {
        standardButtons: QQC2.DialogButtonBox.Close
        onRejected: Qt.quit()
    }

    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            Kirigami.Icon {
                source: "dialog-error"
                width: Kirigami.Units.iconSizes.enormous
                height: width
            }
            QQC2.Label {
                Layout.fillWidth: true
                text: i18nc("@info", "An error occurred during installation: %1",
                            DebugRepoEnabler.error !== "" ? DebugRepoEnabler.error : Installer.error)
                wrapMode: Text.Wrap
            }
        }

        QQC2.ScrollView {
            id: view
            Layout.fillWidth: true
            Layout.fillHeight: true
            QQC2.TextArea {
                Layout.fillWidth: true
                readOnly: true
                // Reverse the log so we have a good chance of finding the most recent (and ideally most
                // relevant) information in a screenshot. Users tend to post screenshots right now *shrug*
                text: Logger.log().reverse().join('')
                wrapMode: Text.Wrap
            }
        }
    }
}
