#include "ThermalRunaway.h"

ThermalRunawayState ThermalRunaway::update(float temperature, float setpoint, bool powered)
{
  if(!prevPowerState && powered)
  {
    turnOnTime = millis();
    tempAtTurnOn = temperature;
    Serial.println("Heater turned on");
  }
  if(prevPowerState && !powered)
  {
    Serial.print("Heater turned off: ");Serial.print(temperature - setpoint);Serial.print(" ");
    Serial.println(millis()-turnOnTime);
  }
  prevPowerState = powered;

  if(millis() - prevSampleTime > SAMPLE_INTERVAL)
  {
    float newThermalChangeRate = (temperature - prevSampleTemp) / ((millis() - prevSampleTime)/1000.0);
    prevSampleTime = millis();
    prevSampleTemp = temperature;
    Serial.println(newThermalChangeRate);
  }

  return RUNAWAY_OK;
}