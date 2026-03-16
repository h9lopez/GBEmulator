FROM cosmtrek/air:latest AS air-builder

# Use the official Microsoft C++ Dev Container image
FROM mcr.microsoft.com/devcontainers/cpp:latest

# Install air for hot-reloading using multi-stage copy
COPY --from=air-builder /go/bin/air /usr/local/bin/air

# Set environment variables for Clang compiler
ENV CC=clang
ENV CXX=clang++

# Create a working directory
WORKDIR /app
