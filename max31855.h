#ifndef MAX31855_GLA
#define MAX31855_GLA

#include "Arduino.h"

typedef enum
{
  MAX_ERROR_NONE,
  MAX_ERROR_OPEN_CIRCUIT,
  MAX_ERROR_SHORT_GROUND,
  MAX_ERROR_SHORT_VCC,
} Max38155_error;

typedef struct
{
  float ext_celcius;
  float int_celcius;
  Max38155_error error;
  uint32_t raw;
} Max31855_ret_t;

class Max31855
{
public:
  Max31855();
  Max31855(int cs);
  set_cs(int cs);
  Max31855_ret_t get_temp(void);
private:
  uint32_t read();
  void chip_enabled(bool en);
  int cs;
  bool ready = false;

  uint16_t read_6675();
  Max31855_ret_t get_temp_6675();

  bool read_pin(); 
};

#endif //MAX31855_GLA
