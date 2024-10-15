#include "HeaterController.h"


HeaterController::HeaterController() : zoneCount(0) {}

bool HeaterController::addZone(const char* name, int cs, int out, float kp, float ki, float kd) {
    if (zoneCount < MAX_ZONES) {
        zones[zoneCount].initialize(name, cs, out, kp, ki, kd);
        ++zoneCount;
        return true;
    }
    return false; // Maximum zones reached
}

HeaterZone* HeaterController::findZoneByName(const char* name) {
    for (int i = 0; i < zoneCount; ++i) {
        if (strcmp(zones[i].getZoneName(), name) == 0) {
            return &zones[i];
        }
    }
    return nullptr; // Zone not found
}

bool HeaterController::setZoneTemperature(const char* name, float temp) {
    HeaterZone* zone = findZoneByName(name);
    if (zone) {
        zone->setTemperature(temp);
        return true;
    }
    return false; // Zone not found
}

bool HeaterController::setZoneKi(const char* name, float ki) {
    HeaterZone* zone = findZoneByName(name);
    if (zone) {
        zone->setKi(ki);
        return true;
    }
    return false; // Zone not found
}

bool HeaterController::setZoneKp(const char* name, float kp) {
    HeaterZone* zone = findZoneByName(name);
    if (zone) {
        zone->setKp(kp);
        return true;
    }
    return false; // Zone not found
}

bool HeaterController::turnZoneOn(const char* name) {
    HeaterZone* zone = findZoneByName(name);
    if (zone) {
        zone->turnOn();
        return true;
    }
    return false; // Zone not found
}

bool HeaterController::turnZoneOff(const char* name) {
    HeaterZone* zone = findZoneByName(name);
    if (zone) {
        zone->turnOff();
        return true;
    }
    return false; // Zone not found
}

void HeaterController::processCommand(const char* command) {
    char zoneName[50];
    char parameter[10];
    float value;

    // Parse the command string using sscanf to extract the zone name, parameter (optional), and value
    int parsed = sscanf(command, "\"%49[^\"]\" %9s %f", zoneName, parameter, &value);

    if (parsed == 2)
    {
        value = atof(parameter); // Convert the parameter to float
        setZoneTemperature(zoneName, value);
    }
    if (parsed == 3)
    {
      if (strcmp(parameter, "ki") == 0)
      {
          setZoneKi(zoneName, value);
      }
      if (strcmp(parameter, "kp") == 0)
      {
        setZoneKp(zoneName, value);
      }
      if(strcmp(parameter, "kd") == 0)
      {
        setZoneKd(zoneName, value);
      }
    }
}
