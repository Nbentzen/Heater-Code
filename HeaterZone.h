#ifndef HEATER_ZONE_GLA
#define HEATER_ZONE_GLA
#include "max31855.h"
#include "PID.h"
#include "ThermalRunaway.h"

#define RAMP_RATE .25

#define HYSTERISIS .5

typedef enum {
  HEATER_ERROR_NONE,
  HEATER_ERROR_RUNAWAY,
} HeaterZone_error;

typedef struct {
  Max31855_ret_t TcState;
  float dutyCycle;
  float setPoint;
  float realSetPoint;
  bool isOn;
  bool isPowered;
  HeaterZone_error error;
  float analogPinRead;
} HeaterZoneRet_t;

class HeaterZone {
public:
    HeaterZone::HeaterZone()
    : zoneName(nullptr), cs_pin(-1), out_pin(-1), set_point(0), isOn(false), pid(0, 0, 0), analog_pin(-1), rampRate(0.0), minTemp(25), maxTemp(250) {}

    void initialize(const char* name, int cs, int out, float proportional, float integral, float derivative, int analogPin, float rampRate,
              int minTemp, int maxTemp);
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

    HeaterZoneRet_t update();

    Max31855_ret_t getTemperature() const;

    HeaterZoneRet_t get_state();

private:
    const char* zoneName;
    int cs_pin;
    int out_pin;
    float set_point;
    float real_set_point;
    bool isOn;
    void getThermocoupleState();

    HeaterZoneRet_t state;

    int analog_pin;

    bool powerOn;

    float rampRate;

    uint32_t prev_update_time;

    Max31855 thermocouple;
    Max31855_ret_t thermocouple_state;

    ThermalRunaway thermalRunaway;

    int minTemp;
    int maxTemp;

    PID pid;
};

#endif //HEATER_ZONE_GLA
