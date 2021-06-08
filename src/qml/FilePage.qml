// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2020 Harald Sitter <sitter@kde.org>

import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15 as QQC2
import org.kde.kirigami 2.4 as Kirigami
import org.kde.drkonqi.debug.installer.pk 1.0

Kirigami.ScrollablePage {
    property QtObject file // File instance actually; not registered in type system

    title: file.path

    ColumnLayout {
        Layout.fillWidth: true

        Kirigami.FormLayout {
            Layout.fillWidth: true

            FilePropertyLabel {
                Kirigami.FormData.label: i18nc("@label", "PackageKit ID:")
                Layout.fillWidth: true
                prop: file.packageID
            }

            FilePropertyLabel {
                Kirigami.FormData.label: i18nc("@label", "Source Package:")
                Layout.fillWidth: true
                prop: file.sourcePackage
            }

            FilePropertyLabel {
                Kirigami.FormData.label: i18nc("@label", "Debug PackageKit ID:")
                Layout.fillWidth: true
                prop: file.debugPackageID
            }

            QQC2.Label {
                Kirigami.FormData.label: i18nc("@label package installation status", "Status:")
                Layout.fillWidth: true
                visible: file.debugPackageID !== ""
                text: file.debugPackageInstalled ?
                    i18nc("info:status", "Debug package already installed") :
                    i18nc("info:status", "Debug package not yet installed")
                color: file.debugPackageInstalled ?
                    Kirigami.Theme.positiveTextColor :
                    Kirigami.Theme.negativeTextColor
                wrapMode: Text.Wrap
            }
        }

        QQC2.TextArea {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: file.diagnosticData !== ""
            text: file.diagnosticData
            readOnly: true
        }
    }
}
