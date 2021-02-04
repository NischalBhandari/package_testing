#include <mosquitto.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef WIN32
#  include <unistd.h>
#else
#  include <process.h>
#  define snprintf sprintf_s
#endif


int get_frequency(void)
{ 
	sleep(1); /* Prevent a storm of messages - this pretend sensor works at 1Hz */
	return random()%10000;
}



/* This function pretends to read some data from a sensor and publish it.*/
void publish_sensor_data(struct mosquitto *mosq, char *argv)
{
	char payload[20];
	int temp;
	int rc;
	char topic[20];
	strcpy(topic,argv);
	printf("The topic is %s\n",topic);
	/* Get our pretend data */
	temp = get_frequency();
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
	rc = mosquitto_publish(mosq, NULL,topic, strlen(payload), payload, 2, false);
	printf("published data with data %s\n",payload);
	if(rc != MOSQ_ERR_SUCCESS){
		fprintf(stderr, "Error publishing: %s\n", mosquitto_strerror(rc));
	}
}