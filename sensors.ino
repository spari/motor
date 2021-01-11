#include "sensors.h"

#include <Arduino.h>

#include "local.h"

Sensors::Sensors() {}

void Sensors::init()
{
   pinMode(gpio.current_sensor, INPUT);
}

/*
   My circuit uses a TA17L-05 1:3000 current transformer, with a
   330 ohm burden resistor, input to the ADC of a ESP8266 which has a
   max limit of 3.3V.

   The ADC resolution for the above configuration is approximately 3.2mV
   Peak-to-Peak. This equates to approximately 20mA/bit. So, the *maximum*
   current it can measure is 20*1024 = approximately 20A.

   If the load draws more than 20A, it will cause more than 3.3V to be
   input to the ADC which will damage it. This should be fine, as most
   home submerisble pumps are within 10A, most commonly around 5A.

   Calculation:

   // Burden Resistor.
   Rb = 330

   // Current transformer coil ratio.
   current_transformer_ratio = 3000

   // Volts per bit resolution of ADC.
   volts_per_bit = Vcc/ADC_max_value = 3.3/1024.0

   // Voltage (Peak-to-Peak).
   V_pp = Vn_pp * volts_per_bit

   // Current (Peak-to-Peak).
   I_pp = V_pp/Rb

   // Current (RMS).
   I_rms = I_pp * 1/sqrt(2)

   // Motor Current (RMS).
   I_L_rms = I_rms * current_transformer_ratio

   // Putting it all together.
   I_L_rms = (Vn_pp * volts_per_bit * current_transformer_ratio)/(R * sqrt(2))
*/
float Sensors::get_current()
{
   const float Rb = 330;
   const int current_transformer_ratio = 3000;
   float volts_per_bit = 3.3/1024.0;  // 3.2mV

   /*
      Mains supply sine wave is 50Hz. That is, 50 cycles in 1s.
      FYI, on a NodeMCU ESP8266-12E, this loop takes about 8,000
      samples in 1s to find the max value as peak-to-peak.
      That's about 8000/50 = 160 samples per sine wave.
   */
   int Vn, Vn_pp=0;
   uint32_t start = millis();
   while ((millis()-start) < 100) {
      Vn = analogRead(gpio.current_sensor);
      if (Vn > Vn_pp) {
         Vn_pp = Vn;
      }
   }

   log_debug("Vn_pp = %d", Vn_pp);

   /*
      When motor is switched off there is a stray voltage (@ 3.2mV/bit).
      Ignore and zero it out if Vn_pp < 15. 
   */
   if (Vn_pp < 15) {
      Vn_pp = 0;
   }

   // Motor Current (RMS).
   float current  = (Vn_pp * volts_per_bit * current_transformer_ratio)/(Rb * sqrt(2));

   return current;
}

