FROM gcr.io/jsleeio-containers/alpine-cbuild:latest AS build
ADD . /eatcore
WORKDIR /eatcore
RUN make

FROM scratch
WORKDIR /
COPY --from=build /eatcore/eatcore /eatcore
COPY --from=build /lib/ld-musl-x86_64.so* /lib/
COPY --from=build /usr/lib/libbsd* /usr/lib/
ENTRYPOINT ["/eatcore"]
