CFLAGS=-Wall -ggdb
LDFLAGS=./mosquitto-2.0.1/lib/libmosquitto.so.1 -lsqlite3

.PHONY: all clean

all : mqtt_main

mqtt_main : mqtt_main.o publishingsensor.o getmac.o maketopic.o sqliteprepare.o findchannel.o channelinfo.o
	${CC} $^ -o $@ ${LDFLAGS}

channelinfo.o : channelinfo.c
	${CC} -c $^ -o $@ ${CFLAGS}


findchannel.o : findchannel.c
	${CC} -c $^ -o $@ ${CFLAGS}

sqliteprepare.o : sqliteprepare.c
	${CC} -c $^ -o $@ ${CFLAGS}
maketopic.o : maketopic.c
	${CC} -c $^ -o $@ ${CFLAGS}

getmac.o : getmac.c
	${CC} -c $^ -o $@ ${CFLAGS}

publishingsensor.o : publishingsensor.c
	${CC} -c $^ -o $@ ${CFLAGS} -L./mosquitto-2.0.1/lib -I ./mosquitto-2.0.1/include 
# subscribercallback.o : subscribercallback.c
# 	${CC} -c $^ -o $@ ${CFLAGS} -L./mosquitto-2.0.1/lib -I ./mosquitto-2.0.1/include 

mqtt_main.o : mqtt_main.c
	${CC} -c $^ -o $@ ${CFLAGS} -L./mosquitto-2.0.1/lib -I ./mosquitto-2.0.1/include 

clean : 
	-rm -f *.o mqtt_main
