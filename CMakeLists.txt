# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2017-2020 Harald Sitter <sitter@kde.org>

cmake_minimum_required(VERSION 3.16.3)

set(QT_MIN_VERSION "5.15.0")
set(KF5_MIN_VERSION "5.75")

project(drkonqi-pk-debug-installer)
set(PROJECT_VERSION "1.0")
if(NOT VERSION_STRING_FULL) # May be set by packaging; realistically we have no version
    set(VERSION_STRING_FULL ${PROJECT_VERSION})
endif()

set(DEBUG_REPO_APPSTREAM_IDS ""
    CACHE STRING
        "Semicolon separated list of appstream 'repository' component IDs describing debug repos that need adding")

find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(ECMQtDeclareLoggingCategory)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED Widgets DBus Test Quick)
find_package(KF5CoreAddons ${KF5_MIN_VERSION} REQUIRED)
find_package(KF5I18n ${KF5_MIN_VERSION} REQUIRED)
find_package(KF5Declarative ${KF5_MIN_VERSION} REQUIRED)
find_package(PackageKitQt5 REQUIRED)
find_package(AppStreamQt5 1.0.2 REQUIRED)

# Disable legacy stuff to get rid of some deprecation warnings.
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050e00)

# Standardish sanity definitions we tend to use in KDE software.
add_definitions(
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_URL_CAST_FROM_STRING
    -DQT_NO_CAST_FROM_BYTEARRAY
    -DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT
)

if(BUILD_TESTING)
# None here right now. A bit tricky to test anything here without an abstraction interface towards PK.
#  add_subdirectory(autotests)
endif()

ki18n_install(po)

add_subdirectory(src)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
