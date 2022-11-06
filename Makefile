CFLAGS = -Wall -Wextra -Wsign-conversion

sig: ./archive/signals_and_pipes.c
	@${CC} $? ${CFLAGS} -o $@
	@./$@
	@rm $@