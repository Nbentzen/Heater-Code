#include "HeaterZone.h"

void HeaterZone::initialize(const char* name, int cs, int out,
                      float proportional, float integral, float derivative) {
    zoneName = name;
    cs_pin = cs;
    out_pin = out;
    kp = proportional;
    ki = integral;
    kd = derivative;

    thermocouple = Max31855(cs_pin);
}

void HeaterZone::setTemperature(float temp) {
    temperature = temp;
}

float HeaterZone::getTemperature() const {
    return temperature;
}

void HeaterZone::setKi(float integral) {
    ki = integral;
}

void HeaterZone::setKp(float proportional) {
  kp = proportional;
}

void HeaterZone::setKd(float derivative) {
  kd = derivative;
}

float HeaterZone::getKi() const {
    return ki;
}

float HeaterZone::getKp() const {
    return kp;
}

float HeaterZone::getKd() const {
  return kd;
}

void HeaterZone::turnOn() {
    isOn = true;
}

void HeaterZone::turnOff() {
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
