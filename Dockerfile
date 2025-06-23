FROM alpine:3 AS build
RUN apk add llvm clang libbsd-dev make 
ADD . /eatcore
WORKDIR /eatcore
RUN make

FROM scratch
WORKDIR /
COPY --from=build /eatcore/eatcore /eatcore
COPY --from=build /lib/ld-musl-* /lib/
COPY --from=build /usr/lib/libmd*so* /usr/lib/
COPY --from=build /usr/lib/libbsd*so* /usr/lib/
ENTRYPOINT ["/eatcore"]
