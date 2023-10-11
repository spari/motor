#ifndef controller_mqtt_h
#define controller_mqtt_h

/*
 * MQTT payload for TOPIC_POWER.
 */
const char* MotorOff = "off";
const char* MotorOn = "on";

/*
 * MQTT Topics 
 */
const char* TOPIC_START         = "/start";
const char* TOPIC_STOP          = "/stop";
const char* TOPIC_AMPS          = "/amps";
const char* TOPIC_RESULT        = "/result";
const char* TOPIC_RESTART       = "/restart";
const char* TOPIC_SYSINFO       = "/sysinfo";
const char* TOPIC_HELLO         = "/hello";

void print_sysinfo();
void publish_current();

#endif

