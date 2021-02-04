#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef WIN32
#  include <unistd.h>
#else
#  include <process.h>
#  define snprintf sprintf_s
#endif

#include <mosquitto.h>
#include <sqlite3.h>


#define mqtt_host "localhost"
#define mqtt_broker "192.168.11.9"
#define mqtt_port 1883
#define db_query "INSERT INTO mqtt_log (topic,payload) VALUES (?,?)"


void connect_callback(struct mosquitto *mosq, void *obj, int result){
	printf("Connection done\n");


}

void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message){
	char data[30];
	char topic[10];
	// there was a problem in sqlite due to direct assignment as maybe due to '' so we
	//used characters to denote the const void* value 
	memcpy(data,message->payload,30);
	memcpy(topic,message->topic,10);
	// sqlite3_bind_blob(sqlite3 stmt*, int index, const void* value , int n, void (*) (void*))
	
	sqlite3_bind_text(res,2,data,10,SQLITE_STATIC);
	sqlite3_bind_text(res,1,topic,20,SQLITE_STATIC);

	printf("The message is %s  && topic is %s\n",data,topic);
	printf("\n");
	int rc=sqlite3_step(res);
	if(rc == SQLITE_DONE){
		printf("added to the database: \n");
		printf("\n");
	}
	else{
		printf("Execution failed %s\n",sqlite3_errmsg(connection));
	}
}