CFLAGS=-Wall
OS := $(shell uname -s)
ifeq ($(OS),Linux)
	LDLIBS += -lbsd
	CFLAGS += -DLINUX
endif

eatcore: eatcore.c

docker: clean
	docker build -t eatcore -f Dockerfile .

install: eatcore
	cp eatcore /usr/local/bin

clean:
	$(RM) eatcore

indent:
	gindent -nut -i2 -l90 -hnl -kr eatcore.c
