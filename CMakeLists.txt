# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2017 Harald Sitter <sitter@kde.org>

project(pk-drkonqi-debug-installer)

cmake_minimum_required(VERSION 2.8.12)

find_package(ECM 0.0.14 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

set(REQUIRED_QT_VERSION 5.7)
find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Widgets DBus Test Quick)
find_package(KF5CoreAddons 5.0.0 REQUIRED)
find_package(KF5I18n 5.0.0 REQUIRED)
find_package(PackageKitQt5 REQUIRED)

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050e00)

# Either use integration or autotesting. Do not use both as
# that screws up results and prolongs build time in vagrant.
if ($ENV{INTEGRATION_TESTING})
  add_subdirectory(integrationtests)
else()
# None here right now.
#  add_subdirectory(autotests)
endif()

add_subdirectory(src)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
