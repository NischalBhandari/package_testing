#include <signal.h>
#include <stdio.h>
#include <string.h>

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

static int run = 1;
sqlite3 *connection;
static sqlite3_stmt *res;
char *err_msg = 0;
char *sql= "DROP TABLE IF EXISTS mqtt_log;"
"CREATE TABLE mqtt_log(Id INTEGER PRIMARY KEY, topic TEXT, payload TEXT);";
#define db_query "INSERT INTO mqtt_log (topic, payload) VALUES (?,?)"

void handle_signal(int s){
	run = 0;
	sqlite3_finalize(res);
	sqlite3_close(connection);
}

void connect_callback(struct mosquitto *mosq, void *obj, int result){
	printf("Connection done\n");

}

void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message){
	char data[30];
	// sqlite3_bind_blob(sqlite3 stmt*, int index, const void* value , int n, void (*) (void*))
	
	sqlite3_bind_blob(res,1,message->topic,strlen(message->topic),SQLITE_STATIC);
	sqlite3_bind_blob(res,2,message->payload,strlen(message->payload),SQLITE_STATIC);
	memcpy(data,message->payload,28);
	printf("The message is %s\n",data);
	int rc=sqlite3_step(res);
	if(rc == SQLITE_DONE){
		printf("added to the database: \n");
	}
	else{
		printf("Execution failed %s\n",sqlite3_errmsg(connection));
	}
}

int main(int argc, char *argv[]){


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
		mosquitto_subscribe(mosq, NULL, "hell", 0);
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