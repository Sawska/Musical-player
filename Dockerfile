FROM alpine:3.17.0 AS build


RUN apk update && \
    apk add --no-cache \
        build-base=0.5-r3 \
        cmake=3.24.3-r0 \
        postgresql-dev=14.5-r0 \
        libpqxx-dev=7.6.0-r0 \
        sfml-audio=2.5.0-r0 \
        sfml-system=2.5.0-r0 \
        wxWidgets=3.1.7-r0


WORKDIR /musical_player


COPY src/ ./src
COPY CMakeLists.txt .


RUN cmake -S . -B build && \
    cmake --build build --target MyApp


FROM alpine:3.17.0


RUN apk add --no-cache \
        boost1.80=1.80.0-r3 \
        postgresql-libs=14.5-r0 \
        libpqxx=7.6.0-r0 \
        sfml-audio=2.5.0-r0 \
        sfml-system=2.5.0-r0 \
        wxWidgets=3.1.7-r0


COPY --from=build /musical_player/build/MyApp /usr/local/bin/MyApp


ENTRYPOINT [ "/usr/local/bin/MyApp" ]
