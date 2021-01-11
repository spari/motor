#ifndef motor_h
#define motor_h

/*
 * Flags.
 */
//#define DEBUG_LOG // Turn on debug output.

/*
 * Custom Libs.
 */
#include <signaller.h>
#include <logger.h>
#include <wifi_utils.h>
#include <time_utils.h>
#include <ota_utils.h>
#include <mqtt_utils.h>

#include "sensors.h"
#include "dol_starter.h"
#include "controller.h"

const char* CODE_VERSION = "1.0";

/*
 * See each struct definition for respective default values.
 */
struct Config {
   struct wifi_conf wifi;
   struct time_conf time;
   struct mqtt_conf mqtt;
   struct dol_starter_conf dol_starter;
   struct controller_conf controller;
};

/*
 * Pin assignments.
 */
struct dol_starter_gpio DolStarter::gpio = {
   .start_button = D1,
   .stop_button = D2
};

struct sensors_gpio Sensors::gpio = {
   .current_sensor = A0
};

struct signaller_gpio Signaller::gpio = {
   .sysok_led = BUILTIN_LED,
   .status_led = BUILTIN_LED,
   .buzzer = -1
};

Signaller signaller;
OtaUtils ota_utils;
TimeUtils time_utils;
MqttUtils mqtt_utils;
WifiUtils wifi_utils;

Sensors sensors;
DolStarter dol_starter;
Controller controller;

#endif
