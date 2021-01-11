#ifndef dol_starter_h 
#define dol_starter_h 


struct dol_starter_conf {
   /*
    * Used to determine if DOL Starter has switched on.
    *
    * DOL starter are almost always accompanied by a protection circuit
    * (overload, single-phasing, phase-reversal, phase-imbalance). So either 
    * it will run (at its normal load current which is definitely > 3A) or it 
    * won't. So set on_current to 3A. 
    *
    * See notes in Sensors::get_current() for current limitations.
    */
   float on_current = 3;
};

struct dol_starter_gpio {
   uint8_t start_button;
   uint8_t stop_button;
};

class DolStarter {
   public:
      const struct dol_starter_conf *conf;
      static struct dol_starter_gpio gpio;

      DolStarter();

      void init(const struct dol_starter_conf *conf);
      void start();
      void stop();
      bool is_started();
};

#endif
