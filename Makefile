CFLAGS=-Wall
OS := $(shell uname -s)
ifeq ($(OS),Linux)
	LDLIBS += -lbsd
	CFLAGS += -DLINUX
endif

eatcore: eatcore.c

docker: clean
	docker build -t eatcore -f Dockerfile.eatcore .

install: eatcore
	cp eatcore /usr/local/bin

clean:
	$(RM) eatcore

test:
	echo $(shell uname -s)

indent:
	gindent -nut -i2 -kr eatcore.c
