#include "HeaterController.h"


bool HeaterController::addZone(const char* name, int cs, int out, float kp, float ki, float kd, int analogPin, float rampRate, int minTemp, int maxTemp) {
    if (zoneCount < MAX_ZONES) {
        zones[zoneCount].initialize(name, cs, out, kp, ki, kd, analogPin, rampRate, minTemp, maxTemp);

        ++zoneCount;
        set_name_len(strlen(name));
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
        if(temp > 300){temp = 300;}
        if(zone->getSetPoint() < 1.0)
        {
          zone->turnOn();
        }
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

void HeaterController::printHelp()
{
  Serial.println("Example usage:");
  Serial.println("mass_spec 150 -> set this zone to 150");
  Serial.println("if the zone is off this will turn it on too");
  Serial.println("mass_spec off -> turn off this zone but keep setpoint");
  Serial.println("mass_spec on -> turn this zone back on");
  Serial.println("off -> turn all heaters off");
  Serial.println("on -> turn all heaters on");
  delay(5000);
}

void HeaterController::allZonesOnOff(bool on)
{
  for(int i = 0; i < zoneCount; i++)
  {
    if(on)
    {
      zones[i].turnOn();
    }
    else
    {
      zones[i].turnOff();
    }
  }
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

    if(strcmp(name_str, "help\n") == 0)
    {
      printHelp();
      return;
    }

    if(strcmp(name_str, "off\n") == 0)
    {
      allZonesOnOff(false);
      return;
    }

    if(strcmp(name_str, "on\n") == 0)
    {
      allZonesOnOff(true);
      return;
    }


    for(int i = 0; *command+i != NULL; i++)
    {
      if(isupper((unsigned char)*command))
      {
        Serial.println("Lower case only!");
        break;
      }
    }

    if(cmd_str == NULL && val_str == NULL)
    {
      // this would be a good place to do a debug print function
      Serial.println("type help for help");
      return false;
    }

    if(val_str == NULL)
    {
      if(isNumeric(cmd_str))
      {
        if (strchr(cmd_str, '.') != NULL)
        {
          value = atof(cmd_str);
        }
        else
        {
          value = (float)atoi(cmd_str);
        }
        if(setZoneSetPoint(name_str, value))
        {
          return HEATER_CTRL_SUCCESS;
        }
        else
        {
          return HEATER_CTRL_INVALID_NAME;
        }
        
      }
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

    if(!isNumeric(val_str)){Serial.println("Expected a number");return HEATER_CTRL_INVALID_VALUE;}

    if (strchr(val_str, '.') != NULL)
    {
      value = atof(val_str);
    }
    else
    {
      value = (float)atoi(val_str);
    }

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

bool HeaterController::isReady()
{
  if(millis() < 5000){return false;}
  if(millis() > prevIteration + HEATER_LOOP_TIME)
  {
    return true;
  }
  return false;
}

void HeaterController::startUpdate()
{
  prevIteration = millis();
  if(updateFinished)
  {
    zonesToUpdate = zoneCount;
    updateFinished = false;
  }
}

void HeaterController::finishUpdate()
{
  updateFinished = true;
}

HeaterCtrl_ret_t HeaterController::update()
{
  HeaterCtrl_ret_t result;
  // startUpdate();
  // if(zonesToUpdate > 0)
  // {
  //   result.zoneState = zones[zoneCount - zonesToUpdate].update();
  //   result.zoneName = zones[zoneCount - zonesToUpdate].getZoneName();
  //   result.heaterCtrl_error = HEATER_CTRL_SUCCESS;
  //   zonesToUpdate--;
  //   result.remainingZones = zonesToUpdate;
  // }
  // if(zonesToUpdate == 0)
  // {
  //   finishUpdate();
  // }
  for(int i = 0; i < zoneCount; i++)
  {
    zones[i].update();
  }
  return result;
}

void HeaterController::updateDisplay()
{
  for(int i = 0; i < zoneCount; i++)
  {
    HeaterZoneRet_t zoneState = zones[i].get_state();
    update_lcd(zones[i].getZoneName(), i, zoneState.TcState.ext_celcius, zoneState.setPoint, zoneState.realSetPoint, zoneState.isOn, zoneState.isPowered, zoneState.TcState.error);
    //Serial.println(zoneState.TcState.error);
  }
}
//void update_lcd(const char* name, int line_no float temp, float set_temp, bool powered, int error)
