#ifndef THERMAL_RUNAWAY_GLA
#define THERMAL_RUNAWAY_GLA

#include "Arduino.h"

#define RUNAWAY_INTERVAL 15000
#define SAMPLE_INTERVAL 2000

typedef enum
{
  RUNAWAY_OK,
  RUNAWAY_DETECTED,
} ThermalRunawayState;

class ThermalRunaway {
  public:
    ThermalRunawayState update(float temperature, float setpoint, bool powered);

  private:
    float tempAtTurnOn;
    uint32_t turnOnTime;
    uint32_t prevSampleTime;
    float prevSampleTemp;
    bool prevPowerState;
    ThermalRunawayState state;
    float thermalChangeRate;

  private:
    void armOverShoot(float peakTemp, float setPoint, uint32_t time);
    void evaluateOvershoot(float temperature);
};

#endif //THERMAL_RUNAWAY_GLA