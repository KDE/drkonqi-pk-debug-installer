# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
# SPDX-FileCopyrightText: 2017 Harald Sitter <sitter@kde.org>

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
