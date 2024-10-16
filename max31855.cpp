#include "max31855.h"
#include <math.h>
#include <SPI.h>

#define MAX31855_FAULT_OPEN       0x01
#define MAX31855_FAULT_SHORT_GND  0x02
#define MAX31855_FAULT_SHORT_VCC  0x03
#define MAX31855_FAULT_UNION      0x07

Max31855::Max31855(int cs)
{
  cs = cs;
  pinMode(cs, OUTPUT);
  chip_enabled(false);
}

uint32_t Max31855::read()
{
  chip_enabled(true);
  uint32_t result = 0;
  for(int i = 0; i < 4; i++)
  {
    result |= SPI.transfer(0x00);
    result <<= 8;
  }
  chip_enabled(false);
  return result;
}

void Max31855::chip_enabled(bool en)
{
  digitalWrite(cs, en ? LOW : HIGH);
}

Max31855_ret_t Max31855::get_temp()
{
  Max31855_ret_t result = {NAN, MAX_ERROR_NONE};
  uint32_t data = read();
  if (data & MAX31855_FAULT_OPEN) {result.error = MAX_ERROR_OPEN_CIRCUIT;}
  if (data & MAX31855_FAULT_SHORT_GND) {result.error = MAX_ERROR_SHORT_GROUND;}
  if (data & MAX31855_FAULT_SHORT_VCC) {result.error = MAX_ERROR_SHORT_VCC;}

  if (data & MAX31855_FAULT_UNION) {return result;}

  result.int_celcius = (float)((data >> 4) & 0x7FF);
  result.int_celcius *= 0.0625;
  if ((data >> 4) & 0x800){result.int_celcius *= -1.0;}

  if(data & 0x80000000)
  {
    data = 0xFFFFC000 | ((data >> 18) & 0x00003FFF);
  }
  else
  {
    data >>= 18;
  }
  result.ext_celcius = (float)data;
  result.ext_celcius *= 0.25;

  return result;
}
