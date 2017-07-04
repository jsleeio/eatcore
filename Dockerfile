FROM alpine:latest
ADD . /eatcore
WORKDIR /eatcore
RUN \
  apk update \
  && apk add --virtual .build-deps gcc make musl-dev \
  && make clean install \
  && apk del .build-deps

CMD [ "./eatcore" ]
