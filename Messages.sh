#!/usr/bin/env bash
# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2017 Harald Sitter <sitter@kde.org>

$EXTRACTRC `find . -name \*.rc` >> rc.cpp
$XGETTEXT `find . -name '*.cpp'` -o $podir/pk-drkonqi-debug-installer.pot
rm -f *.cpp
