#include <mosquitto.h>
#include <time.h>
#include "getmac.h"
#include "mqtt_main.h"

int get_frequency(void)
{ 
	sleep(1); /* Prevent a storm of messages - this pretend sensor works at 1Hz */
	return random()%10000;
}



/* This function pretends to read some data from a sensor and publish it.*/
void publish_sensor_data(struct mosquitto *mosq, char *argv, struct Channel *Ch)
{//	struct Channel payload;
	char data[data_buffer_size];
	// time_t raw_time;
	// struct tm *timeinfo;
	// time(&raw_time);
	// timeinfo=localtime(&raw_time);
	char mac_addr[20];
	memset(mac_addr,0,20);
	int rc;
	char topic[30];
	strcpy(topic,argv);
	get_mac(mac_addr);
	get_frequency();
	memset(data,0,data_buffer_size);
	for(int i=0;i<13;i++){
		snprintf(data,sizeof(data),"%s %d %d %d\n",mac_addr,Ch[i].frequency,Ch[i].active_time,Ch[i].busy_time);
	//	printf(" frequency: %d active time: %d && busy time : %d\n",Ch[i].frequency,Ch[i].active_time,Ch[i].busy_time);
		rc = mosquitto_publish(mosq, NULL,topic, strlen(data), data, 2, false);
		if(rc != MOSQ_ERR_SUCCESS){
			fprintf(stderr, "Error publishing: %s\n", mosquitto_strerror(rc));
		}

	}
	//snprintf(data, sizeof(data), "%s-%d-%d-%s",mac_addr,(*payload).frequency,(*payload).channel,asctime(timeinfo));

	/* Publish the message
	 * mosq - our client instance
	 * *mid = NULL - we don't want to know what the message id for this message is
	 * topic = "example/temperature" - the topic on which this message will be published
	 * payloadlen = strlen(payload) - the length of our payload in bytes
	 * payload - the actual payload
	 * qos = 2 - publish with QoS 2 for this example
	 * retain = false - do not use the retained message feature for this message
	 */

}