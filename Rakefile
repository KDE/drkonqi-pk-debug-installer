# Copyright © 2017 Harald Sitter <sitter@kde.org>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License or (at your option) version 3 or any later version
# accepted by the membership of KDE e.V. (or its successor approved
# by the membership of KDE e.V.), which shall act as a proxy
# defined in Section 14 of version 3 of the license.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
