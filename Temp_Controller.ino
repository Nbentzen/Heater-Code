#include <SPI.h>
#include "HeaterController.h"


// TempTextInterface interface;

void setup() {
  SPI.begin();
  Serial.begin(9600);

}

void loop() {
  // char input[256];
  // memset(input, 0, sizeof(input));
  // int ptr = 0;

  // TempSetting_t result;

  

  // while(Serial.available())
  // {
  //   if(interface.addChar(Serial.read()))
  //   {
  //     result = interface.parseCommand();
  //     Serial.print(result.command);
  //     Serial.print(" ");
  //     Serial.println(result.temperature);
  //   }
  // }
}
