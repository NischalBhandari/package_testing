CFLAGS=-Wall -ggdb
LDFLAGS=./mosquitto-2.0.1/lib/libmosquitto.so.1 -lsqlite3

.PHONY: all clean

all : mqtt_main

mqtt_main : mqtt_main.o
	${CC} $^ -o $@ ${LDFLAGS}

mqtt_main.o : mqtt_main.c
	${CC} -c $^ -o $@ ${CFLAGS} -L./mosquitto-2.0.1/lib -I ./mosquitto-2.0.1/include 

clean : 
	-rm -f *.o mqtt_main
