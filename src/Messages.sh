#!/bin/sh
# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2020 Harald Sitter <sitter@kde.org>

$XGETTEXT `find -name \*.cpp -o -name \*.qml -o -name \*.js` -o $podir/drkonqi-pk-debug-installer.pot
