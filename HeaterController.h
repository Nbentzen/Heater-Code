#ifndef HEATER_CONTROL_GLA
#define HEATER_CONTROL_GLA

#include "HeaterZone.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define HEATER_CTRL_SUCCESS 0
#define HEATER_CTRL_ERR_GENERIC 1
#define HEATER_CTRL_ERR_STR_OVERFLOW 2
#define HEATER_CTRL_INVALID_NAME 3
#define HEATER_CTRL_INVALID_CMD 4
#define HEATER_CTRL_INVALID_VALUE 5


const int MAX_ZONES = 5;

class HeaterController {
public:
    HeaterController();
    bool addZone(const char* name, int cs, int out, float kp, float ki, float kd);
    bool setZoneSetPoint(const char* name, float temp);
    bool setZoneKi(const char* name, float ki);
    bool setZoneKp(const char* name, float kp);
    bool setZoneKd(const char* name, float kd);
    bool turnZoneOn(const char* name);
    bool turnZoneOff(const char* name);
    void displayStatus() const;
    int processCommand(const char* command);

private:
    HeaterZone zones[MAX_ZONES];
    int zoneCount;
    int commandSafetyCheck(const char* command);

    HeaterZone* findZoneByName(const char* name);
    bool isNumeric(const char* str);
};

#endif //HEATER_CONTROL_GLA
