CFLAGS=-Wall -Werror
eatcore: eatcore.c

docker: clean
	docker build -t eatcore -f Dockerfile.eatcore .

install: eatcore
	cp eatcore /usr/local/bin

clean:
	$(RM) eatcore
