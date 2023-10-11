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

#include "config_manager.h"
#include "sensors.h"
#include "dol_starter.h"
#include "controller.h"
#include "ap_switch.h"

const char* CODE_VERSION = "2.1";

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

struct ap_switch_gpio ApSwitch::gpio = {
   .ap_button = D7
};

struct signaller_gpio Signaller::gpio = {
   .sysok_led = LED_BUILTIN,
   .status_led = LED_BUILTIN
/*
   .sysok_led = D3,
   .status_led = D4
*/
};

Signaller signaller;
OtaUtils ota_utils;
TimeUtils time_utils;
MqttUtils mqtt_utils;
WifiUtils wifi_utils;

Sensors sensors;
DolStarter dol_starter;
ApSwitch ap_switch;
Controller controller;

#endif
