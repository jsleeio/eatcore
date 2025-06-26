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
	gindent -linux -hnl -l110 eatcore.c
