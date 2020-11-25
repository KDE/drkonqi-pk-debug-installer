# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2017-2020 Harald Sitter <sitter@kde.org>

add_definitions(-DTRANSLATION_DOMAIN=\"drkonqi-pk-debug-installer\")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/Version.h)

set(installdbgsymbols_SRCS
    DebugResolver.cpp
    FileResolver.cpp
    Installer.cpp
    Package.cpp
    DebugPackage.cpp
    Logger.cpp
    File.cpp
)

ecm_qt_declare_logging_category(installdbgsymbols_SRCS
    HEADER Debug.h
    IDENTIFIER INSTALLER
    CATEGORY_NAME "org.kde.drkonqi.pk-debug-installer"
    DESCRIPTION "PackageKit based debug installer for drkonqi"
)

add_executable(drkonqi-pk-debug-installer ${installdbgsymbols_SRCS} main.cpp qml/qml.qrc)

target_compile_definitions(drkonqi-pk-debug-installer
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(drkonqi-pk-debug-installer
    PRIVATE
        Qt5::Widgets
        Qt5::Quick
        KF5::CoreAddons
        KF5::I18n
        PK::packagekitqt5
        KF5::Declarative)

install(TARGETS drkonqi-pk-debug-installer ${INSTALL_TARGETS_DEFAULT_ARGS} )
