#include "controller.h"

#include <Arduino.h>
#include <TimeAlarms.h>
#include <time.h>

#include "sensors.h"
#include "dol_starter.h"
#include "controller_mqtt.h"
#include "local.h"

/*
   C wrappers to C++ methods for passing to callbacks.
*/
void _start_motor() { controller.start_motor(); }
void _stop_motor() { controller.stop_motor(); }

Controller::Controller() {}

void Controller::init(const struct controller_conf *conf)
{
   this->conf = conf;
}

void Controller::start_motor()
{
   /*
      Always check using is_motor_started() which checks for motor current
      via current sensor. Don't check the flag motor_started, as the motor
      could have been started manually (via physical switch).
   */
   if (is_motor_started()) {
      log_info("Motor already started.");
      return;
   }

   time_t ctime = now();
   if (difftime(ctime, last_stop_time) <= conf->restart_delay*60) {
      log_info("Motor was just run. Please wait %d min before restarting.", conf->restart_delay);
      return;
   }

   log_info("--> Starting Motor.");
   dol_starter.start();

   if (is_motor_started()) {
      motor_started = true;
      publish_current();
      // Set a trigger to stop the motor after given duration.
      stop_motor_id = Alarm.timerOnce(conf->duration*60, _stop_motor);
   }
   else {
      log_warn("Failed to start motor.");
   }
}

void Controller::stop_motor()
{
   Alarm.free(stop_motor_id);
   stop_motor_id = dtINVALID_ALARM_ID;

   if (is_motor_started()) {
      last_stop_time = now();
   }

   log_info("--> Stopping Motor.");
   dol_starter.stop();

   motor_started = false;

   publish_current();
}

bool Controller::is_motor_started()
{
   return dol_starter.is_started();
}
