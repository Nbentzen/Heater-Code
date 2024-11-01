#include "HeaterZone.h"
#include <Math.h>

void HeaterZone::initialize(const char* name, int cs, int out,
                      float proportional, float integral, float derivative) {
    zoneName = name;
    cs_pin = cs;
    out_pin = out;
    pid = PID(proportional, integral, derivative);
    thermocouple.set_cs(cs);
}

void HeaterZone::setSetPoint(float temp) {
    set_point = temp;
}

float HeaterZone::getSetPoint() const {
    return set_point;
}

void HeaterZone::setKp(float proportional) {
  pid.setKp(proportional);
}

void HeaterZone::setKi(float integral) {
  pid.setKi(integral);
}

void HeaterZone::setKd(float derivative) {
  pid.setKd(derivative);
}

float HeaterZone::getKp() const {
    return pid.getKp();
}

float HeaterZone::getKi() const {
    return pid.getKi();
}

float HeaterZone::getKd() const {
  return pid.getKd();
}

void HeaterZone::turnOn() {
    if(!isOn)
    {
      Serial.print(zoneName);
      Serial.println(" on");
    }
    isOn = true;
}

void HeaterZone::turnOff() {
    if(isOn)
    {
      Serial.print(zoneName);
      Serial.println(" off");
    }
    isOn = false;
}

bool HeaterZone::isZoneOn() const {
    return isOn;
}

const char* HeaterZone::getZoneName() const {
    return zoneName;
}

int HeaterZone::getCsPin() const {
    return cs_pin;
}

int HeaterZone::getOutPin() const {
    return out_pin;
}

Max31855_ret_t HeaterZone::getTemperature() const {
    return thermocouple_state;
}

HeaterZoneRet_t HeaterZone::update()
{
    float dT = (millis() - prev_update_time)/1000.0;
    prev_update_time = millis();
    
    if(real_set_point < set_point)
    {
      if(real_set_point < 25.0){real_set_point = 30.1;}
      real_set_point += dT*RAMP_RATE;
    }
    if(real_set_point > set_point)
    {
      real_set_point = set_point;
    }
    HeaterZoneRet_t result;
    thermocouple_state = thermocouple.get_temp();
    float duty_cycle = NAN;
    if(thermocouple_state.error == MAX_ERROR_NONE)
    {
      duty_cycle = pid.compute(thermocouple_state.ext_celcius);
    }
    result.TcState = thermocouple_state;
    result.dutyCycle = duty_cycle;
    result.setPoint = real_set_point;
    result.isOn = isOn;

    if(!isOn){real_set_point = 0.0;}

    if(isOn && result.TcState.error == MAX_ERROR_NONE)
    {
      if(powerOn)
      {
        if(thermocouple_state.ext_celcius > HYSTERISIS / 2 + real_set_point)
        {
          powerOn = false;
        }
      }
      else
      {
        if(thermocouple_state.ext_celcius < real_set_point - HYSTERISIS / 2)
        {
          powerOn = true;
        }
      }
      //thermalRunaway.update(thermocouple_state.ext_celcius, set_point, powerOn);
      digitalWrite(out_pin, powerOn ? HIGH : LOW);
    }
    else
    {
      digitalWrite(out_pin, LOW);
    }

    return result;
}