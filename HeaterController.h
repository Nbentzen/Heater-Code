#ifndef HEATER_CONTROL_GLA
#define HEATER_CONTROL_GLA

#include "HeaterZone.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "LCDControl.h"

#define HEATER_LOOP_TIME 50

typedef enum
{
  HEATER_CTRL_SUCCESS = 0,
  HEATER_CTRL_ERR_GENERIC = 1,
  HEATER_CTRL_ERR_STR_OVERFLOW = 2,
  HEATER_CTRL_INVALID_NAME = 3,
  HEATER_CTRL_INVALID_CMD = 4,
  HEATER_CTRL_INVALID_VALUE = 5,  
} HeaterCtrl_error;

typedef struct
{
  HeaterCtrl_error heaterCtrl_error;
  int remainingZones;
  const char* zoneName;
  HeaterZoneRet_t zoneState;
} HeaterCtrl_ret_t;

const int MAX_ZONES = 5;

class HeaterController {
public:
    HeaterController::HeaterController() : zoneCount(0), prevIteration(0), zonesToUpdate(0), updateFinished(false) {}
    bool addZone(const char* name, int cs, int out, float kp, float ki, float kd, int analogPin, float rampRate, int minTemp, int maxTemp);
    bool setZoneSetPoint(const char* name, float temp);
    bool setZoneKi(const char* name, float ki);
    bool setZoneKp(const char* name, float kp);
    bool setZoneKd(const char* name, float kd);
    bool turnZoneOn(const char* name);
    bool turnZoneOff(const char* name);
    void displayStatus() const;
    int processCommand(const char* command);
    bool isReady();
    HeaterCtrl_ret_t update();

    void updateDisplay();

private:
    HeaterZone zones[MAX_ZONES];
    int zoneCount;
    int commandSafetyCheck(const char* command);

    HeaterZone* findZoneByName(const char* name);
    bool isNumeric(const char* str);
    uint32_t prevIteration;

    int zonesToUpdate;
    bool updateFinished = false;
    void startUpdate();
    void finishUpdate();

    void allZonesOnOff(bool on);
    
    void printHelp();
};

#endif //HEATER_CONTROL_GLA
