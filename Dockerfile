# Build Stage
FROM ubuntu:22.04 AS build

# Set the time zone to Etc/UTC
ENV DEBIAN_FRONTEND=noninteractive

# Install tzdata to configure time zone
RUN apt-get update && apt-get install -y tzdata

# Set the time zone and reconfigure tzdata
RUN echo "Etc/UTC" > /etc/timezone && \
    dpkg-reconfigure -f noninteractive tzdata

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    wget \
    git \
    postgresql \
    postgresql-contrib \
    libpq-dev \
    postgresql-server-dev-all \
    libsfml-dev \
    libwxgtk3.0-gtk3-dev \
    && rm -rf /var/lib/apt/lists/*

# Install pqxx from source if necessary
RUN wget https://github.com/jtv/libpqxx/archive/refs/tags/7.7.5.tar.gz && \
    tar -xzf 7.7.5.tar.gz && \
    cd libpqxx-7.7.5 && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make && \
    make install

# Install Catch2
RUN wget https://github.com/catchorg/Catch2/archive/v2.13.10.tar.gz && \
    tar -xzf v2.13.10.tar.gz && \
    cd Catch2-2.13.10 && \
    cmake -S . -B build && \
    cmake --build build --target install

# Set the working directory
WORKDIR /musical_player

# Copy your project files into the container
COPY . .

# Remove any existing CMake cache files
RUN rm -rf build

# Configure paths
ENV PKG_CONFIG_PATH="/usr/local/lib/pkgconfig:/usr/lib/x86_64-linux-gnu/pkgconfig"
ENV CMAKE_PREFIX_PATH="/usr/local;/usr"

# Build your project
RUN cmake -S . -B build && \
    cmake --build build --target MyApp

# Runtime Stage
FROM ubuntu:22.04

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    libpq5 \
    libsfml-dev \
    libwxgtk3.0-gtk3 \
    && rm -rf /var/lib/apt/lists/*

# Copy the built application from the build stage
# COPY --from=build /musical_player/build/MyApp /usr/local/bin/MyApp

# Set the entry point
ENTRYPOINT [ "/usr/local/bin/MyApp" ]
