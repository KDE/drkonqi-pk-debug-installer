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

require_relative 'test_helper'

class IntegrationTest < Minitest::Test
  def setup
    @bin = "#{ENV.fetch('BUILD_DIR')}/src/drkonqi-pk-debug-installer"
    assert(File.exist?(@bin))

    # Make sure we aren't getting locked out by packagekitd. Pre-installer setups
    # will want access to apt.
    system('sudo killall -9 packagekitd')
  end

  def test_dolphin
    assert(Apt.purge('dolphin-dbgsym'))
    assert(Apt.install('dolphin'))
    assert(!Apt.installed?('dolphin-dbgsym'))
    assert(system(@bin, '/usr/bin/dolphin'))
    assert(Apt.installed?('dolphin-dbgsym'))
  end

  def test_qtcore
    assert(Apt.purge('libqt5core5a-dbgsym'))
    assert(system(@bin, '/usr/lib/x86_64-linux-gnu/libQt5Core.so.5'))
    assert(Apt.installed?('libqt5core5a-dbgsym'))
  end

  def test_plasma_workspace
    # plasma-workspace still has a -dbg pckage. Hell if I know why.
    # NB: may need sobumpery if the test fails.
    assert(Apt.install('libtaskmanager6'))
    assert(Apt.purge('plasma-workspace-dbg'))
    assert(system(@bin, '/usr/lib/x86_64-linux-gnu/libtaskmanager.so.6'))
    assert(Apt.installed?('plasma-workspace-dbg'))
  end
end
