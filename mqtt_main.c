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
#define mqtt_port 1883
#define db_query "INSERT INTO mqtt_log (topic,payload) VALUES (?,?)"

static int run = 1;
sqlite3 *connection;
static sqlite3_stmt *res;
char *err_msg = 0;
char *sql= "DROP TABLE IF EXISTS mqtt_log;"
"CREATE TABLE mqtt_log(topic TEXT, payload TEXT);";


/* Callback called when the client receives a CONNACK message from the broker. */
void on_connect(struct mosquitto *mosq, void *obj, int reason_code)
{
	/* Print out the connection result. mosquitto_connack_string() produces an
	 * appropriate string for MQTT v3.x clients, the equivalent for MQTT v5.0
	 * clients is mosquitto_reason_string().
	 */
	printf("on_connect: %s\n", mosquitto_connack_string(reason_code));
	if(reason_code != 0){
		/* If the connection fails for any reason, we don't want to keep on
		 * retrying in this example, so disconnect. Without this, the client
		 * will attempt to reconnect. */
		mosquitto_disconnect(mosq);
	}

	/* You may wish to set a flag here to indicate to your application that the
	 * client is now connected. */
}


	
/* Callback called when the client knows to the best of its abilities that a
 * PUBLISH has been successfully sent. For QoS 0 this means the message has
 * been completely written to the operating system. For QoS 1 this means we
 * have received a PUBACK from the broker. For QoS 2 this means we have
 * received a PUBCOMP from the broker. */
void on_publish(struct mosquitto *mosq, void *obj, int mid)
{
	printf("Message with mid %d has been published.\n", mid);
}


int get_temperature(void)
{ 
	sleep(1); /* Prevent a storm of messages - this pretend sensor works at 1Hz */
	return random()%1000;
}

/* This function pretends to read some data from a sensor and publish it.*/
void publish_sensor_data(struct mosquitto *mosq)
{
	char payload[20];
	int temp;
	int rc;

	/* Get our pretend data */
	temp = get_temperature();
	/* Print it to a string for easy human reading - payload format is highly
	 * application dependent. */
	snprintf(payload, sizeof(payload), "%d", temp);

	/* Publish the message
	 * mosq - our client instance
	 * *mid = NULL - we don't want to know what the message id for this message is
	 * topic = "example/temperature" - the topic on which this message will be published
	 * payloadlen = strlen(payload) - the length of our payload in bytes
	 * payload - the actual payload
	 * qos = 2 - publish with QoS 2 for this example
	 * retain = false - do not use the retained message feature for this message
	 */
	rc = mosquitto_publish(mosq, NULL, "mypayload", strlen(payload), payload, 2, false);
	printf("published data with data %s\n",payload);
	if(rc != MOSQ_ERR_SUCCESS){
		fprintf(stderr, "Error publishing: %s\n", mosquitto_strerror(rc));
	}
}




void handle_signal(int s){
	run = 0;
}

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

int main(int argc, char *argv[]){

	//creates a child process which can keep listening to the topics 
	if(fork()==0){

	int rc=0;
	char clientid[24];
	struct mosquitto *mosq;
	
	/*
	calls a function when you press ctrl + c or use kill PID
	SIGINT - Interrupt interactive attention request sent to the program
	SIGTERM - Terminate, termination request sent to the program
	*/
	signal(SIGINT, handle_signal);
	signal(SIGTERM, handle_signal);
	mosquitto_lib_init();
	rc = sqlite3_open("test.db",&connection);
	if (rc != SQLITE_OK){
		fprintf(stderr," Error: unable to open database\n");
		printf("%s\n",sqlite3_errmsg(connection));
		sqlite3_close(connection);
		return 1;
	}

	rc = sqlite3_exec(connection, sql, 0, 0, &err_msg);
	printf("Created a table \n");
	rc = sqlite3_prepare_v2(connection, db_query, -1, &res, 0);
	/*
	copies the character c to the first n characters of the string
	pointed by the argument str
	void *memset( void *str, int c, size_t n);
	This sets the clientid string with 0 values for 24 positions 
	*/
	memset(clientid,1,24);
	/*
	struct mosquitto *mosquitto_new(const char *id, bool clean_start, void *userdata)
	*/
	char *id="N";
	mosq = mosquitto_new(id,true,connection);
	if(mosq){
		mosquitto_connect_callback_set(mosq,connect_callback);
/*		
libmosq_EXPORT void mosquitto_message_callback_set(struct mosquitto *mosq, void (*on_message)(struct mosquitto *, void *, const struct mosquitto_message *));

 * Function: mosquitto_message_v5_callback_set
 *
 * Set the message callback. This is called when a message is received from the
 * broker.
 *
 * Parameters:
 *  mosq -       a valid mosquitto instance.
 *  on_message - a callback function in the following form:
 *               void callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
 *
 * Callback Parameters:
 *  mosq -    the mosquitto instance making the callback.
 *  obj -     the user data provided in <mosquitto_new>
 *  message - the message data. This variable and associated memory will be
 *            freed by the library after the callback completes. The client
 *            should make copies of any of the data it requires.
 *  props - list of MQTT 5 properties, or NULL
 *
 * See Also:
 * 	<mosquitto_message_copy>
 */
		mosquitto_message_callback_set(mosq,message_callback);

		rc = mosquitto_connect(mosq, mqtt_host, mqtt_port, 60);
		mosquitto_subscribe(mosq, NULL, "mypayload", 0);
		while (run){
			rc = mosquitto_loop(mosq, -1, 1);
			if(run && rc){
				sleep(20);
				mosquitto_reconnect(mosq);
			}
		}
		mosquitto_destroy(mosq);
	}
	sqlite3_finalize(res);
	sqlite3_close(connection);
	return 0;

}
else{
		struct mosquitto *mosq;
	int rc;

	/* Required before calling other mosquitto functions */
	mosquitto_lib_init();

	/* Create a new client instance.
	 * id = NULL -> ask the broker to generate a client id for us
	 * clean session = true -> the broker should remove old sessions when we connect
	 * obj = NULL -> we aren't passing any of our private data for callbacks
	 */
	mosq = mosquitto_new(NULL, true, NULL);
	if(mosq == NULL){
		fprintf(stderr, "Error: Out of memory.\n");
		return 1;
	}

	/* Configure callbacks. This should be done before connecting ideally. */
	mosquitto_connect_callback_set(mosq, on_connect);
	mosquitto_publish_callback_set(mosq, on_publish);

	/* Connect to test.mosquitto.org on port 1883, with a keepalive of 60 seconds.
	 * This call makes the socket connection only, it does not complete the MQTT
	 * CONNECT/CONNACK flow, you should use mosquitto_loop_start() or
	 * mosquitto_loop_forever() for processing net traffic. */
	rc = mosquitto_connect(mosq, "localhost", 1883, 60);
	if(rc != MOSQ_ERR_SUCCESS){
		mosquitto_destroy(mosq);
		fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
		return 1;
	}

	/* Run the network loop in a background thread, this call returns quickly. */
	rc = mosquitto_loop_start(mosq);
	if(rc != MOSQ_ERR_SUCCESS){
		mosquitto_destroy(mosq);
		fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
		return 1;
	}

	/* At this point the client is connected to the network socket, but may not
	 * have completed CONNECT/CONNACK.
	 * It is fairly safe to start queuing messages at this point, but if you
	 * want to be really sure you should wait until after a successful call to
	 * the connect callback.
	 * In this case we know it is 1 second before we start publishing.
	 */
	while(1){
		publish_sensor_data(mosq);
	}

	mosquitto_lib_cleanup();
	return 0;
	
}
return 0;

}