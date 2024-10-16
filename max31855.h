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
} Max31855_ret_t;

class Max31855
{
public:
  Max31855(int cs);
  Max31855_ret_t get_temp(void);
private:
  uint32_t read();
  void chip_enabled(bool en);
  int cs;
};

#endif //MAX31855_GLA
