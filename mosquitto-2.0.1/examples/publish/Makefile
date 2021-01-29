CFLAGS=-Wall -ggdb
LDFLAGS=../../lib/libmosquitto.so.1

.PHONY: all clean
all: mosquitto_publish

mosquitto_publish: basic-1.o
	${CC} $^ -o $@ ${LDFLAGS}

basic-1.o : basic-1.c
	${CC} -c $^ -o $@ ${CFLAGS} -L ../../lib -I ../../include

clean:
	-rm -f *.o mosquitto_publish
