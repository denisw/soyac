# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure(2) do |config|
  config.vm.box = 'f500/ubuntu-lucid64'

  config.vm.provider :virtualbox do |v|
    # The box has USB 2.0 enabled by default, thus requiring the
    # VirtualBox Extension Pack. Remove this requirement by turning
    # USB off - we don't need it.
    v.customize ['modifyvm', :id, '--usb', 'off']
  end

  config.vm.synced_folder '.', '/soyac'

  config.vm.provision 'shell', privileged: false, inline: <<-SHELL
    sudo apt-get update

    sudo apt-get install -y \
      bison \
      build-essential \
      flex \
      g++-4.1 \
      gdb \
      libboost-dev \
      libboost-filesystem-dev \
      libboost-regex-dev \
      libboost-system-dev \
      libcppunit-dev \
      libgc-dev \
      libicu-dev \
      libpopt-dev \
      libsigc++-2.0-dev \
      scons

   # LLVM 2.5 doesn't build on Ubuntu 10.04's GCC 4.4;
   # use 4.1 instead.
   sudo update-alternatives --quiet \
    --install /usr/bin/g++ g++ /usr/bin/g++-4.1 30

    cd /home/vagrant
    wget http://llvm.org/releases/2.5/llvm-2.5.tar.gz
    tar xzf llvm-2.5.tar.gz

    cd llvm-2.5
    ./configure
    make OPTIMIZE_OPTION=-O0
    sudo make install

    cd /soyac
    scons
    sudo scons install
  SHELL
end
