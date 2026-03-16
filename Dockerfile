FROM cosmtrek/air:latest AS air-builder

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

# Install air for hot-reloading using multi-stage copy (no curl/sh required)
COPY --from=air-builder /go/bin/air /usr/local/bin/air

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
