FROM ubuntu:latest

# Set noninteractive installation to avoid tzdata prompts
ENV DEBIAN_FRONTEND=noninteractive

# Install System Dependencies required for vcpkg, Clang, and CMake
RUN apt-get update && apt-get install -y \
    build-essential \
    clang \
    cmake \
    curl \
    zip \
    unzip \
    tar \
    pkg-config \
    autoconf \
    autoconf-archive \
    automake \
    libtool \
    libltdl-dev \
    git \
    && rm -rf /var/lib/apt/lists/*

# Setup vcpkg globally
RUN git clone https://github.com/microsoft/vcpkg.git /vcpkg && \
    /vcpkg/bootstrap-vcpkg.sh

# Install air for hot-reloading (standard in this workspace)
RUN curl -sSfL https://raw.githubusercontent.com/air-verse/air/master/install.sh | sh -s -- -b /usr/local/bin

# Set environment variables for Clang compiler
ENV CC=clang
ENV CXX=clang++

# Create a working directory
WORKDIR /app

# The remaining build steps will typically be run manually during local development
# Example usage:
# docker build -t gbemulator-dev .
# docker run -it -v $(pwd):/app gbemulator-dev
# > cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/vcpkg/scripts/buildsystems/vcpkg.cmake
# > cmake --build build --config Release
