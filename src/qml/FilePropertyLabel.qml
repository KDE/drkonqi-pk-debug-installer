// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2020 Harald Sitter <sitter@kde.org>

import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2
import org.kde.kirigami 2.4 as Kirigami

QQC2.Label {
    property string prop: ""
    property bool broken: {
        console.log(file.resolved)
        console.log(prop === "")
        return file.resolved && prop === ""
    }

    function resolvingString() {
        if (broken) {
            return i18nc("@info error", "Failed to resolve data!")
        } else if (prop === "") {
            return i18nc("@info place holder while waiting for actual data to load", "...")
        }
        return prop
    }

    color: broken ? Kirigami.Theme.negativeTextColor :
                    Kirigami.Theme.textColor
    text: prop !== "" ? prop : resolvingString()
    wrapMode: Text.Wrap
}

