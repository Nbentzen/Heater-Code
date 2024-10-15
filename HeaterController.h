#ifndef HEATER_CONTROL_GLA
#define HEATER_CONTROL_GLA

#include "HeaterZone.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

const int MAX_ZONES = 5;

class HeaterController {
public:
    HeaterController();
    bool addZone(const char* name, int cs, int out, float kp, float ki, float kd);
    bool setZoneTemperature(const char* name, float temp);
    bool setZoneKi(const char* name, float ki);
    bool setZoneKp(const char* name, float kp);
    bool setZoneKd(const char* name, float kd);
    bool turnZoneOn(const char* name);
    bool turnZoneOff(const char* name);
    void displayStatus() const;
    void processCommand(const char* command);

private:
    HeaterZone zones[MAX_ZONES];
    int zoneCount;

    HeaterZone* findZoneByName(const char* name);
};

#endif //HEATER_CONTROL_GLA
