#ifndef HEATER_ZONE_GLA
#define HEATER_ZONE_GLA
#include "max31855.h"

class HeaterZone {
public:
    HeaterZone() : zoneName(nullptr), cs_pin(0), out_pin(0), kp(0.0f), ki(0.0f), kd(0.0f), temperature(0.0f), isOn(false), thermocouple(0) {}

    void initialize(const char* name, int cs, int out, float proportional, float integral, float derivative);
    void setTemperature(float temp);
    float getTemperature() const;
    void setKi(float integral);
    float getKi() const;
    void setKp(float proportional);
    float getKp() const;
    void setKd(float derivative);
    float getKd() const;
    void turnOn();
    void turnOff();
    bool isZoneOn() const;
    const char* getZoneName() const;

    int getCsPin() const;
    int getOutPin() const;

private:
    const char* zoneName;
    int cs_pin;
    int out_pin;
    float kp;
    float ki;
    float kd;
    float temperature;
    bool isOn;

    Max31855 thermocouple;
};

#endif //HEATER_ZONE_GLA
