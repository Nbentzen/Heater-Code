#ifndef HEATER_ZONE_GLA
#define HEATER_ZONE_GLA
#include "max31855.h"
#include "PID.h"

class HeaterZone {
public:
    HeaterZone() : zoneName(nullptr), cs_pin(0), out_pin(0), kp(0.0f), ki(0.0f), kd(0.0f), set_point(0.0f), isOn(false), thermocouple(0)
    {
        pid.setKp(kp);
        pid.setKi(ki);
        pid.setKd(kd);
    }

    void initialize(const char* name, int cs, int out, float proportional, float integral, float derivative);
    void setSetPoint(float temp);
    float getSetPoint() const;
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

    void refresh();

    Max31855_ret_t getTemperature() const;

private:
    const char* zoneName;
    int cs_pin;
    int out_pin;
    float kp;
    float ki;
    float kd;
    float set_point;
    bool isOn;
    void getThermocoupleState();

    Max31855 thermocouple;
    Max31855_ret_t thermocouple_state;

    PID pid;
};

#endif //HEATER_ZONE_GLA
