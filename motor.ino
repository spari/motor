#include "motor.h"

#include <TimeAlarms.h>
#include <ConfigManager.h>

#include "local.h"

extern struct Config config;
AlarmID_t StartMotorAlarmId = dtINVALID_ALARM_ID;

void setup()
{
   Serial.begin(115200);
   Serial.println();

   init_config_manager();

   signaller.init();
   ota_utils.init();

   /*
      If not configured, wait for system to be configured.
      Else proceed with the normal setup.
   */
   if (config.configured != true) {
      Serial.println("-- Not Configured --");
      return;
   }

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
   StartMotorAlarmId = Alarm.alarmRepeat(config.controller.start_time, 0, 0, _start_motor);
}

void loop()
{
   extern ConfigManager configManager;

   Alarm.delay(0);

   configManager.loop();

   if (config.configured == true) {
      time_utils.reconnect(10);
      mqtt_utils.reconnect();
      mqtt_utils.loop();
      ota_utils.loop();

      // Alarm might not have been set if time was not set, so retry setting it.
      if (StartMotorAlarmId == dtINVALID_ALARM_ID) {
         log_info("Retrying setting StartMotorAlarm.");
         StartMotorAlarmId = Alarm.alarmRepeat(config.controller.start_time, 0, 0, _start_motor);
      }

      signaller.ok();
   }
   else {
      /*
         Wait for system to be configured.
      */
      signaller.warn(0b1000);
   }
}

