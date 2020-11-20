# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2017 Harald Sitter <sitter@kde.org>

if(POLICY CMP0028)
  cmake_policy(SET CMP0028 OLD)
endif()
remove_definitions(-DQT_NO_CAST_FROM_ASCII)

include(ECMAddTests)
include(ECMCoverageOption)

########### unittests ###############

add_test(NAME integration COMMAND ruby ${CMAKE_CURRENT_SOURCE_DIR}/integration_test.rb)
