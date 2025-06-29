FROM alpine:3 AS build
RUN apk add clang libbsd-dev make 
ADD . /eatcore
WORKDIR /eatcore
RUN make

FROM scratch
LABEL org.opencontainers.image.url=https://github.com/jsleeio/eatcore
LABEL org.opencontainers.image.source=https://github.com/jsleeio/eatcore
LABEL org.opencontainers.image.description="eatcore leaks memory in an incremental, controlled fashion"
LABEL org.opencontainers.image.licenses=MIT
WORKDIR /
COPY --from=build /eatcore/eatcore /eatcore
COPY --from=build /lib/ld-musl-* /lib/
COPY --from=build ["/usr/lib/libbsd*so*", "/usr/lib/libmd*so*", "/usr/lib/"]
ENTRYPOINT ["/eatcore"]
