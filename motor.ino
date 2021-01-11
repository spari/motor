#include "motor.h"

#include <TimeAlarms.h>

#include "local.h"

#define LOCAL_MQTT

struct Config config;

/*
   Called every second. Keep-alive/reconnect functions are put here.
*/
void timer1sec_callback()
{
   time_utils.reconnect(10); 
   mqtt_utils.reconnect();
   mqtt_utils.loop();
}

/*
   Currently configuration is stored right in code and uses git smudge 
   and clean process to scrub sensitive configuration from code. 
   TODO: Store and read this from a config file stored in SPIFFS.
*/
void init_config()
{
   strcpy(config.wifi.ssid, "{wifi_ssid}"); //scrubbed
   strcpy(config.wifi.password, "{wifi_password}"); //scrubbed

#ifndef LOCAL_MQTT
   strcpy(config.mqtt.server, "{mqtt_server}"); //scrubbed
   config.mqtt.port = atoi("{mqtt_port}"); //scrubbed
   strcpy(config.mqtt.username, "{mqtt_username}"); //scrubbed
   strcpy(config.mqtt.password, "{mqtt_password}"); //scrubbed
#endif

   strcpy(config.mqtt.client_id, "motor0");
   strcpy(config.mqtt.base_topic, "/home-iot/motor0");
}

void setup()
{
   Serial.begin(115200);

   init_config();

   signaller.init();
   wifi_utils.init(&config.wifi);
   ota_utils.init();
   time_utils.init(&config.time);
   mqtt_utils.init(&config.mqtt);
   sensors.init();
   dol_starter.init(&config.dol_starter);
   controller.init(&config.controller);

   print_sysinfo();
   log_info("System Ready.");

   /*
      Start automatically everyday at hour specified by start_time.
   */
   Alarm.alarmRepeat(config.controller.start_time, 0, 0, _start_motor);

   Alarm.timerRepeat(1, timer1sec_callback);
}

void loop()
{
   ota_utils.loop();

   // Must be called at least once in loop() for TimeAlarms to be enabled.
   Alarm.delay(1);

   signaller.ok();
}

