#ifndef controller_h
#define controller_h

#include <TimeAlarms.h>

// For use in callbacks.
void _stop_motor();
void _start_motor();

struct controller_conf {
   /*
    * Motor will be automatically started everyday at this hour.
    * Hour is in 24hr format (range 0-23).
    * Default = 7hr.
    */
   uint8_t start_time = 7;

   /*
    * How long to run the motor. In minutes.
    * Default = 20min.
    */
   uint8_t duration = 20;

   /*
    * Short-cycling a submersible pump can reduce its life-span and even damage
    * it. Enforce a wait of at least 5 minutes before restarting. In minutes.
    */
   uint8_t restart_delay = 5;
};

class Controller {
   public:
      const struct controller_conf *conf;

      Controller();

      void init(const struct controller_conf *conf);
      void start_motor();
      void stop_motor();
      bool is_motor_started();
      time_t last_run_time = 0;

   private:
      int stop_motor_id = dtINVALID_ALARM_ID;
      bool motor_started = false;
};

#endif
