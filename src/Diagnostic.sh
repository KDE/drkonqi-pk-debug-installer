#!/bin/sh
# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
# SPDX-FileCopyrightText: 2021 Harald Sitter <sitter@kde.org>

export LANG=C.UTF-8
unset LANGUAGE

set -x

# $1 is the file path as given to the installer
# $2 is the canonical file path from QFileInfo

readlink /lib # is this a usr merged system?

stat $1
stat $2

readelf -n $1

grep --no-messages --include=maps --recursive $(basename $1) /proc/
