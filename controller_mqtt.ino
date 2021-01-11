#include "controller_mqtt.h"

#include <mqtt_utils.h>
#include "controller.h"
#include "sensors.h"
#include "local.h"

void MqttUtils::subscribe_topics()
{
   mqtt_utils.subscribe(TOPIC_POWER);
   mqtt_utils.subscribe(TOPIC_AMPS);
   mqtt_utils.subscribe(TOPIC_RESTART);
   mqtt_utils.subscribe(TOPIC_SYSINFO);
   mqtt_utils.subscribe(TOPIC_HELLO);
}

/*
   WARNING: The PubSubClient publish() clobbers the content of char* topic 
   argument.
   This is problematic if publish() is called second time within the 
   callback. For that reason, since log_mqtt() uses publish(), use 
   log_serial() instead of log_mqtt() here.
*/
void MqttUtils::callback(const char* topic, byte* payload, unsigned int size)
{
   /*
      If we are going to call publish() within the callback, then make a 
      copy of payload, as it gets clobbered when calling publish().
   */
   const int MAX_PAYLOAD_SIZE = 20;
   char payload_copy[MAX_PAYLOAD_SIZE] = "";
   if (size > 0 && size < MAX_PAYLOAD_SIZE) {
      memcpy(payload_copy, payload, size);
      payload_copy[size] = '\0';
   }

   // Strip off base_topic by shifting pointer.
   const char* subtopic = topic + strlen(mqtt_utils.conf->base_topic);

   if (!strcmp(subtopic, TOPIC_HELLO)) {
      mqtt_utils.publish(TOPIC_RESULT, "namaste");
   }
   else if (!strcmp(subtopic, TOPIC_POWER)) {
      if (!strcmp(payload_copy, MotorOn)) {
         controller.start_motor();
      }
      else if (!strcmp(payload_copy, MotorOff)) {
         controller.stop_motor();
      }
      else {
         log_warn("Invalid power param (valid power params: %s,%s): '%s'", MotorOff, MotorOn, payload_copy);
      }
   }
   else if (!strcmp(subtopic, TOPIC_AMPS)) {
      publish_current();
   }
   else if (!strcmp(subtopic, TOPIC_RESTART)){
      log_info("Restarting device.\n");
      delay(1000);
      ESP.restart();
   }
   else if (!strcmp(subtopic, TOPIC_SYSINFO)) {
      print_sysinfo();
   }
   else {
      log_warn("Invalid topic: '%s'", topic);
   }
}

void print_sysinfo()
{
   log_info("curr-time:         %s", time_utils.ctime_str().c_str());
   log_info("version:           %s", CODE_VERSION);
   log_info("ip-addr:           %s", wifi_utils.get_ip().c_str());
   log_info("mac-addr:          %s", wifi_utils.get_mac().c_str());
   log_info("start-time:        %02dhrs", config.controller.start_time);
   log_info("duration:          %02dm", config.controller.duration);
   log_info("restart-delay:     %02dm", config.controller.restart_delay);
   log_info("on-current:        %02.2fA", config.dol_starter.on_current);
   log_info("uptime:            %s", time_utils.uptime_str().c_str());

   log_debug("flash-size:        %4dKB", ESP.getFlashChipRealSize()/1024);
   log_debug("flash-used:        %4dKB", ESP.getSketchSize()/1024);
   log_debug("flash-free:        %4dKB", ESP.getFreeSketchSpace()/1024);
   log_debug("ram-free:          %4dKB", ESP.getFreeHeap()/1024);
   log_debug("time-ntp_server:   '%s'", config.time.ntp_server);
   log_debug("time-timezone:     '%s'", config.time.timezone);
   log_debug("mqtt-server:       '%s'", config.mqtt.server);
   log_debug("mqtt-port:         %d", config.mqtt.port);
   log_debug("mqtt-client_id:    '%s'", config.mqtt.client_id);
   log_debug("mqtt-base_topic:   '%s'", config.mqtt.base_topic);
   log_debug("mqtt-log_topic:    '%s'", config.mqtt.log_topic);
}

void publish_current()
{
   const int MAX_RESULT_SIZE = 80;
   char result_topic[MAX_RESULT_SIZE];

   sprintf(result_topic, "%s%s", TOPIC_RESULT, TOPIC_AMPS);

   mqtt_utils.publish(result_topic, String(sensors.get_current()).c_str());
}

