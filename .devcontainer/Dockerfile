FROM mcr.microsoft.com/devcontainers/cpp:debian13

# Install project dependencies
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
        bison \
        clangd \
        libboost1.83-dev \
        libboost-filesystem1.83-dev \
        libboost-program-options1.83-dev \
        libboost-regex1.83-dev \
        libcppunit-dev \
        libgc-dev \
        llvm-19-dev \
        just \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*
