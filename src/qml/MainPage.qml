// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2020 Harald Sitter <sitter@kde.org>

import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15 as QQC2
import org.kde.kirigami 2.4 as Kirigami
import org.kde.drkonqi.debug.installer.pk 1.0

Kirigami.ScrollablePage {
    title: i18nc("@title", "Debug Symbols Helper")

    Component.onCompleted: Installer.resolve() // start resolivng data

    footer: QQC2.DialogButtonBox {
        QQC2.Button {
            id: installButton
            enabled: Installer.ready && !Installer.installing
            action: Kirigami.Action {
                text: i18nc("@action:button all = debug symbols for which packages are available",
                            "Install All")
                iconName: "run-install"
                onTriggered: {
                    Installer.install()
                    installButton.visible = false
                }
            }
            QQC2.DialogButtonBox.buttonRole: QQC2.DialogButtonBox.AcceptRole
        }
        QQC2.Button {
            action: Kirigami.Action {
                enabled: !Installer.installing
                text: Installer.installed ?
                        i18nc("@action:button", "Close") :
                        i18nc("@action:button", "Cancel")
                onTriggered: Qt.quit()
            }
            QQC2.DialogButtonBox.buttonRole: QQC2.DialogButtonBox.RejectRole
        }
    }

    ListView {
        id: view
        model: Installer.files
        currentIndex: -1 // don't select anything by default
        delegate: Kirigami.AbstractListItem {
            id: listItem
            // TODO do we need this?
            // width: parent ? parent.width : implicitWidth

            contentItem: RowLayout {
                QQC2.Label {
                    Layout.fillWidth: true
                    text: modelData.path
                    elide: Text.ElideMiddle
                }
                // Indicates either resolution in progress or install in progress.
                QQC2.BusyIndicator {
                    id: busy
                    visible: running
                    running: !modelData.resolved || Installer.installing
                }

                // FIXME: icon diagnostic state ought to be on the file page somehow

                // don't make the line jump; keep consistent size with indicator
                // and place the icon within at an implict width equal to
                // the busy indicator's effective width. This effectively centers
                // the icon while keeping the same spatial requirements.
                Item {
                    visible: !busy.running
                    width: busy.width
                    height: busy.height

                    Kirigami.Icon {
                        implicitWidth: busy.width
                        anchors.centerIn: parent
                        source: {
                            if (modelData.packageID === "" || modelData.debugPackageID === "") {
                                return "package-broken"
                            }
                            if (modelData.debugPackageInstalled) {
                                return "installed"
                            }
                            return "package-install"
                        }
                    }
                }
            }

            action: Kirigami.Action {
                onTriggered: {
                    if (pageStack.depth > 1) {
                        if (pageStack.lastItem.file == modelData) {
                            // Clicked this entry twice, treat this as deselecting again.
                            view.currentIndex = -1
                            pageStack.pop()
                            return
                        }

                        pageStack.pop()
                    }
                    pageStack.push("qrc:/FilePage.qml", {"file": modelData});
                }
            }
        }
    }
}
