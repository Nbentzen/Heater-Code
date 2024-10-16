#include "HeaterController.h"

HeaterController::HeaterController() : zoneCount(0) {}

bool HeaterController::addZone(const char* name, int cs, int out, float kp, float ki, float kd) {
    if (zoneCount < MAX_ZONES) {
        zones[zoneCount].initialize(name, cs, out, kp, ki, kd);

        Serial.print("Added zone ");Serial.println(name);

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

bool HeaterController::setZoneSetPoint(const char* name, float temp) {
    HeaterZone* zone = findZoneByName(name);
    if (zone) {
        zone->setSetPoint(temp);
        Serial.print("Set temp " );Serial.print(name);Serial.print(" ");Serial.println(temp);
        return true;
    }
    return false; // Zone not found
}

bool HeaterController::setZoneKp(const char* name, float kp) {
    HeaterZone* zone = findZoneByName(name);
    if (zone) {
        Serial.print("Set kp " );Serial.print(name);Serial.print(" ");Serial.println(kp);
        zone->setKp(kp);
        return true;
    }
    return false; // Zone not found
}

bool HeaterController::setZoneKi(const char* name, float ki) {
    HeaterZone* zone = findZoneByName(name);
    if (zone) {
        Serial.print("Set ki " );Serial.print(name);Serial.print(" ");Serial.println(ki);
        zone->setKi(ki);
        return true;
    }
    return false; // Zone not found
}

bool HeaterController::setZoneKd(const char* name, float kd) {
    HeaterZone* zone = findZoneByName(name);
    if (zone) {
        Serial.print("Set kd " );Serial.print(name);Serial.print(" ");Serial.println(kd);
        zone->setKi(kd);
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

bool HeaterController::isNumeric(const char* str)
{
  bool decimalFound = false;
  bool digitFound = false;
  bool firstChar = true;

  if(str == NULL || *str == '\0'){return false;}

  while(*str)
  {
    Serial.print(str);
    if(*str == ".")
    {
      if(decimalFound){return false;}
      decimalFound = true;
    }
    if(*str >= '0' && *str < '9'){digitFound = true;}
    if(*str == "-" && !firstChar){return false;}

    firstChar = false;
    str++;
  }
  return digitFound;
}

int HeaterController::processCommand(const char* command) {
    Serial.print("recv: ");Serial.println(command);

    float value = 0.0f;

    if(strlen(command) > 50){Serial.println("overflow"); return HEATER_CTRL_ERR_STR_OVERFLOW;}

    // Parse the command string using sscanf to extract the zone name, parameter (optional), and value
    char *name_str = strtok(command, " ");
    char *cmd_str  = strtok(NULL, " ");
    char *val_str  = strtok(NULL, " ");

    if(name_str == NULL){return false;}

    if(cmd_str == NULL && val_str == NULL)
    {
      // this would be a good place to do a debug print function
      return false;
    }

    if(val_str == NULL)
    {
      if(strcmp(cmd_str, "off") == 0)
      {
        return turnZoneOff(name_str) ? HEATER_CTRL_SUCCESS : HEATER_CTRL_INVALID_NAME;
      }
      if(strcmp(cmd_str, "on") == 0)
      {
        return turnZoneOn(name_str) ? HEATER_CTRL_SUCCESS : HEATER_CTRL_INVALID_NAME;
      }
      return HEATER_CTRL_INVALID_CMD;
    }

    if(!isNumeric(val_str)){return HEATER_CTRL_INVALID_VALUE;}

    if (strchr(val_str, '.') != NULL)
    {
      value = atof(val_str);
    }
    else
    {
      value = (float)atoi(val_str);
    }
    Serial.print("value: ");Serial.println(value);



    setZoneSetPoint(name_str, value) ? HEATER_CTRL_SUCCESS : HEATER_CTRL_ERR_GENERIC;

    if (strcmp(cmd_str, "ki") == 0)
    {
        return setZoneKi(name_str, value) ? HEATER_CTRL_SUCCESS : HEATER_CTRL_ERR_GENERIC;
    }
    if (strcmp(cmd_str, "kp") == 0)
    {
      return setZoneKp(name_str, value) ? HEATER_CTRL_SUCCESS : HEATER_CTRL_ERR_GENERIC;
    }
    if(strcmp(cmd_str, "kd") == 0)
    {
      return setZoneKd(name_str, value) ? HEATER_CTRL_SUCCESS : HEATER_CTRL_ERR_GENERIC;
    }

    return HEATER_CTRL_INVALID_CMD;
}

