#ifndef sensors_h
#define sensors_h

struct sensors_gpio {
   uint8_t current_sensor;
};

class Sensors {
   public:
      static struct sensors_gpio gpio;

      Sensors();

      void init();
      float get_current();
};

#endif
