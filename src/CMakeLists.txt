# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2017 Harald Sitter <sitter@kde.org>

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/Version.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(installdbgsymbols_SRCS
    DebugResolver.cpp
    FileResolver.cpp
    Installer.cpp
)

add_executable(qmlinstaller ${installdbgsymbols_SRCS} main.new.cpp qml.qrc)

# add_executable(drkonqi-pk-debug-installer ${installdbgsymbols_SRCS} main.cpp)
add_library(testlib STATIC  ${installdbgsymbols_SRCS})

# target_link_libraries(drkonqi-pk-debug-installer
#     Qt5::DBus
#     Qt5::Widgets
#     KF5::CoreAddons
#     KF5::I18n
#     PK::packagekitqt5)
target_link_libraries(qmlinstaller
    PRIVATE
        Qt5::DBus
        Qt5::Widgets
        Qt5::Quick
        KF5::CoreAddons
        KF5::I18n
        PK::packagekitqt5)
target_link_libraries(testlib
    Qt5::DBus
    Qt5::Widgets
    KF5::CoreAddons
    KF5::I18n
    PK::packagekitqt5)

    target_compile_definitions(qmlinstaller
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

# install(TARGETS drkonqi-pk-debug-installer ${INSTALL_TARGETS_DEFAULT_ARGS} )
