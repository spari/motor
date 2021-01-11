#include "dol_starter.h"

#include <Arduino.h>

#include "local.h"

/*
   Relay input uses active-low.
*/
const int ButtonOn = LOW;
const int ButtonOff = HIGH;

DolStarter::DolStarter() {}

void DolStarter::init(const struct dol_starter_conf *conf)
{
   this->conf = conf;

   pinMode(gpio.start_button, OUTPUT);
   pinMode(gpio.stop_button, OUTPUT);

   digitalWrite(gpio.start_button, ButtonOff);
   digitalWrite(gpio.stop_button, ButtonOff);

   log_debug("1. current=%f, on_current=%f", sensors.get_current(), conf->on_current);
}

void DolStarter::start()
{
   digitalWrite(gpio.start_button, ButtonOn);
   delay(1000);
   digitalWrite(gpio.start_button, ButtonOff);
}

void DolStarter::stop()
{
   digitalWrite(gpio.stop_button, ButtonOn);
   delay(1000);
   digitalWrite(gpio.stop_button, ButtonOff);
}

bool DolStarter::is_started()
{
   log_debug("2. current=%f, on_current=%f", sensors.get_current(), conf->on_current);

   return (sensors.get_current() > conf->on_current);
}


