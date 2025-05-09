# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2017-2021 Harald Sitter <sitter@kde.org>

add_definitions(-DTRANSLATION_DOMAIN=\"drkonqi-pk-debug-installer\")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/BuildConfig.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/BuildConfig.h)

set(installdbgsymbols_SRCS
    DebugResolver.cpp
    FileResolver.cpp
    Installer.cpp
    Package.cpp
    DebugPackage.cpp
    Logger.cpp
    File.cpp
    DebugRepoEnabler.cpp
    DiagnosticResolver.cpp
)

ecm_qt_declare_logging_category(installdbgsymbols_SRCS
    HEADER Debug.h
    IDENTIFIER INSTALLER
    DEFAULT_SEVERITY Debug
    CATEGORY_NAME "org.kde.drkonqi.pk-debug-installer"
    DESCRIPTION "PackageKit based debug installer for drkonqi"
)

add_executable(drkonqi-pk-debug-installer ${installdbgsymbols_SRCS} main.cpp qml/qml.qrc assets.qrc)

target_compile_definitions(drkonqi-pk-debug-installer
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(drkonqi-pk-debug-installer
    PRIVATE
        Qt6::Widgets
        Qt6::Quick
        KF6::CoreAddons
        KF6::I18n
        PK::packagekitqt6
        AppStreamQt
)

install(TARGETS drkonqi-pk-debug-installer ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES org.kde.drkonqi-pk-debug-installer.desktop DESTINATION ${KDE_INSTALL_APPDIR})
