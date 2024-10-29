#include "max31855.h"
#include <math.h>
#include <SPI.h>

#define MAX31855_FAULT_OPEN       0x01
#define MAX31855_FAULT_SHORT_GND  0x02
#define MAX31855_FAULT_SHORT_VCC  0x03
#define MAX31855_FAULT_UNION      0x07

#define DELAY 1000

Max31855::Max31855()
{

}

Max31855::Max31855(int cs)
{
  cs = cs;
  pinMode(cs, OUTPUT);
  chip_enabled(false);
  ready = true;
}

Max31855::set_cs(int _cs)
{
  cs = _cs;
  pinMode(cs, OUTPUT);
  chip_enabled(false);
  ready = true;
}

uint32_t Max31855::read()
{
  if(!ready){return 0;}
  chip_enabled(true);
  uint32_t result = 0;
  uint8_t buf[4];
  for(int i = 0; i < 4; i++)
  {
    buf[i] = 0;
    for(int j = 7; j >= 0; j--)
    {
      digitalWrite(SCK, LOW);
      delayMicroseconds(500);
      if(analogRead(A0) > 100){ buf[i] |= (1 << j);}
      digitalWrite(SCK, HIGH);
      delayMicroseconds(500);
    }
  }

  result = buf[0];
  result <<= 8;
  result |= buf[1];
  result <<= 8;
  result |= buf[2];
  result <<= 8;
  result |= buf[3];

  chip_enabled(false);
  //Serial.println("reaD");
  return result;
}

void Max31855::chip_enabled(bool en)
{
  if(ready){digitalWrite(cs, en ? LOW : HIGH);}
  else{Serial.println("Max31855 not ready");}
  
}

bool Max31855::read_pin()
{
  return digitalRead(MISO);
}

uint16_t Max31855::read_6675()
{
  uint16_t result = 0;
  chip_enabled(true);
  delayMicroseconds(DELAY);

  for(int i = 15; i >= 0; i--)
  {
    result <<= 1;
    digitalWrite(SCK, LOW);
    delayMicroseconds(DELAY);
    if(read_pin()){result++;}
    digitalWrite(SCK, HIGH);
    delayMicroseconds(DELAY);
  }

  chip_enabled(false);
  return result;
}

Max31855_ret_t Max31855::get_temp_6675()
{

  Max31855_ret_t result = {NAN, NAN, MAX_ERROR_NONE, 0};
  uint16_t data = read_6675();
  result.raw = data;
  if(data & 0x04){result.error = 1;}

  data >>= 3;

  result.ext_celcius = data * 0.25;

  return result;

}

Max31855_ret_t Max31855::get_temp()
{
  Max31855_ret_t result = {NAN, NAN, MAX_ERROR_NONE, 0};
  result = get_temp_6675();
  return result;
  Serial.println("nope");
  chip_enabled(true);
  
  uint32_t data = read();
  result.raw = data;
  chip_enabled(false);


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
