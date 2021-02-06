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
//keep findchannel.h before publishing sensor.h
#include "findchannel.h"

#include "maketopic.h"
#include "sqliteprepare.h"
#include "publishingsensor.h"

#include "channelinfo.h"

#define data_buffer_size 500
#define topic_buffer_size 100
#define mqtt_host "localhost"
#define mqtt_broker "192.168.11.9"
#define mqtt_port 1883
#define db_query "INSERT INTO mqtt_log (topic,payload,frequency,mac_addr,active_time) VALUES (?,?,?,?,?)"