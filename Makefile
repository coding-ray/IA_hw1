CFLAGS = -Wall -Wextra -Wsign-conversion

all: client server

client: client.c
	@${CC} $? ${CFLAGS} -o $@

server: server.c
	@${CC} $? ${CFLAGS} -lpthread -o $@

# This will be run before a commit
clear:
	@rm -f client server

sig: ./archive/signals_and_pipes.c
	@${CC} $? ${CFLAGS} -o $@
	@./$@
	@rm $@