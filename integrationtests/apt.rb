# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
# SPDX-FileCopyrightText: 2017 Harald Sitter <sitter@kde.org>

module Apt
  module_function

  def install(*args)
    system('sudo', 'apt', 'install', '-y', '--no-install-recommends', *args)
  end

  def purge(*args)
    system('sudo', 'apt', 'purge', '-y', *args)
  end

  def installed?(pkg)
    `dpkg -s #{pkg}`.strip.include?('Status: install ok installed')
  end
end
