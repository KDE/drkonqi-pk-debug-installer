# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
# SPDX-FileCopyrightText: 2017 Harald Sitter <sitter@kde.org>

require 'fileutils'

BUILD_DEPENDS = %w(cmake libkf5coreaddons-dev build-essential
                   extra-cmake-modules libkf5i18n-dev gettext
                   libpackagekitqt5-dev packagekit-tools).freeze
INTEGRATIONTESTS_DIR = "#{__dir__}/integrationtests"
BUILD_DIR = "#{INTEGRATIONTESTS_DIR}/build".freeze

task :build do
  FileUtils.mkpath(BUILD_DIR)
  system("sudo apt install -y --no-install-recommends #{BUILD_DEPENDS.join(' ')}") || raise
  system('cmake', '../..', '-DKDE_INSTALL_USE_QT_SYS_PATHS=ON',
         '-DCMAKE_INSTALL_PREFIX=/usr', '-DCMAKE_BUILD_TYPE=Debug',
         chdir: BUILD_DIR) || raise
  system('make', "-j#{`nproc`.strip}", chdir: BUILD_DIR) || raise
end

task :unrestrict_polkit do
  puts "Unrestricting polkit's packagekit rules..."
  sh 'sudo cp -rv integrationtests/provision/*.pkla' \
     ' /etc/polkit-1/localauthority/50-local.d/'
  sh 'sudo systemctl restart polkitd'
end

task :test do
  ENV['BUILD_DIR'] = BUILD_DIR
  system('ctest --verbose', chdir: BUILD_DIR) || raise
end
task :test => [:build, :unrestrict_polkit]
