#include "PID.h"
#include "Arduino.h"


void PID::setKp(float kp){kp = kp;}
void PID::setKi(float _ki)
{
    //the maximum error expected is ~200, so assume that a ki that would give
    //less than 10% power when integral is fully saturated is "zero"
    if(_ki < 0.1/200.0){_ki = 0.0;}
    ki = _ki;
    max_integral_value = max_integral_contribution / ki;
}
void PID::setKd(float kd){kd = kd;}

float PID::compute(float temperature)
{
  uint32_t time_now = millis();
  float delta_t = (time_now - previous_time)/1000.0;

  float error = setpoint - temperature;

  float proportional_term = kp*error;

  integral += error * delta_t;
  if(integral < 0.0){integral = 0.0;}
  if(integral > max_integral_value){integral = max_integral_value;}

  float integral_term = ki * integral;

  float derivative = (error - previous_error) / delta_t;
  previous_error = error;

  float derivative_term = kd * derivative;

  float result = proportional_term + integral_term + derivative_term;

  if(result > 1.0){result = 1.0;}
  if(result < 0.0){result = 0.0;}

  return result;
}