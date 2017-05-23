Vagrant.configure(2) do |config|
  config.vm.box = "ubuntu/xenial64"

  config.vm.provision "shell", inline: <<-SHELL
    set -ex
    sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys '444D ABCF 3667 D028 3F89  4EDD E6D4 7362 5575 1E5D'
    echo 'deb http://archive.neon.kde.org/user xenial main' | sudo tee --append /etc/apt/sources.list
    sudo apt update
    sudo apt install --no-install-recommends -y ruby
    echo 'INTEGRATION_TESTING=1' >> /etc/environment
    echo 'cd /vagrant' >> /home/ubuntu/.bashrc
    # Fuck off apt, just fuck off.
    sudo systemctl disable apt-daily.service
  SHELL
end
