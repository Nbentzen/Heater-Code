#include <SPI.h>
#include "HeaterController.h"
#include "PinAssignment.h"
#include <Wire.h>
#include "LCDControl.h"


HeaterController heaterController;
PinAssignment pinAssignment;

// hd44780_I2Cexp lcd(0x27); // declare lcd object: auto locate & auto config expander chip

// #define LCD_COLS 20
// #define LCD_ROWS 4

static volatile int something;
void setup() {
  pinMode(MISO, INPUT_PULLUP);
  pinMode(SCK, OUTPUT);
  pinMode(SS, OUTPUT);
  pinMode(MOSI, OUTPUT);
  digitalWrite(SS, HIGH);
  digitalWrite(SCK, HIGH);

  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  pinMode(29, INPUT_PULLUP);

  pinMode(LED_BUILTIN, OUTPUT);

  lcd_begin();

  Serial.begin(9600);
  while(!Serial)
  {
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
    digitalWrite(LED_BUILTIN, HIGH);
  }

  // lcd.write("GLA temp control");
  // lcd.setCursor(0,1);
  // lcd.write("test");
  // Serial.println("---reset---");

  delay(50);
  heaterController.addZone("sampler",   pinAssignment.HeaterList.SamplerHeater.CsPin, pinAssignment.HeaterList.SamplerHeater.OutPin, 0, 0, 0, A0, 0.0, 25, 250);
  heaterController.addZone("spec", pinAssignment.HeaterList.MassSpecHeater.CsPin, pinAssignment.HeaterList.MassSpecHeater.OutPin, 0, 0, 0, A1, 0.0, 25, 125);
  heaterController.addZone("analyte",   pinAssignment.HeaterList.AnalyteHeater.CsPin, pinAssignment.HeaterList.AnalyteHeater.OutPin, 0, 0, 0, A2, 0.0, 25, 250);
  heaterController.addZone("cap", pinAssignment.HeaterList.CapillaryHeater.CsPin, pinAssignment.HeaterList.CapillaryHeater.OutPin, 0, 0, 0, A3, 0.0, 25, 250);

  pinMode(pinAssignment.HeaterList.MassSpecHeater.CsPin,  OUTPUT);
  pinMode(pinAssignment.HeaterList.AnalyteHeater.CsPin,   OUTPUT);
  pinMode(pinAssignment.HeaterList.SamplerHeater.CsPin,   OUTPUT);

  pinMode(pinAssignment.HeaterList.MassSpecHeater.OutPin,   OUTPUT);
  pinMode(pinAssignment.HeaterList.AnalyteHeater.OutPin,    OUTPUT);
  pinMode(pinAssignment.HeaterList.SamplerHeater.OutPin,    OUTPUT);
  pinMode(pinAssignment.HeaterList.CapillaryHeater.OutPin,  OUTPUT);

  //digitalWrite(pinAssignment.HeaterList.SamplerHeater.OutPin,     HIGH); //left
  // digitalWrite(pinAssignment.HeaterList.MassSpecHeater.OutPin,    HIGH); //1
  // digitalWrite(pinAssignment.HeaterList.AnalyteHeater.OutPin,     HIGH);  //2
  // digitalWrite(pinAssignment.HeaterList.CapillaryHeater.OutPin,   HIGH); // right

  delay(50);

  digitalWrite(pinAssignment.HeaterList.SamplerHeater.OutPin,     LOW);
  digitalWrite(pinAssignment.HeaterList.MassSpecHeater.OutPin,    LOW);
  digitalWrite(pinAssignment.HeaterList.AnalyteHeater.OutPin,     LOW);
  digitalWrite(pinAssignment.HeaterList.CapillaryHeater.OutPin,   LOW);

  digitalWrite(pinAssignment.HeaterList.MassSpecHeater.CsPin,   HIGH);
  digitalWrite(pinAssignment.HeaterList.AnalyteHeater.CsPin,    HIGH);
  digitalWrite(pinAssignment.HeaterList.SamplerHeater.CsPin,    HIGH);
  digitalWrite(pinAssignment.HeaterList.CapillaryHeater.CsPin,  HIGH);


}

char input_text[256];
int ptr;
void loop() {
  delay(50);
  digitalWrite(12, digitalRead(29));
  heaterController.update();
  heaterController.updateDisplay();
  // if(heaterController.isReady())
  // {
  //   Serial.println("------");
  //   while(1)
  //   {
  //     digitalWrite(LED_BUILTIN,     HIGH);
  //     HeaterCtrl_ret_t result = heaterController.update();
  //     Serial.print(result.zoneName); Serial.print(" "); Serial.print(result.zoneState.TcState.ext_celcius);
  //     Serial.print(" / ");Serial.print(result.zoneState.setPoint);Serial.print(" / ");Serial.print(result.zoneState.analogPinRead);Serial.print(" / ");
  //     if(result.zoneState.TcState.error)
  //     {
  //       Serial.print("error: ");
  //       Serial.println(result.zoneState.TcState.error);
  //     }
  //     else
  //     {
  //       if(result.zoneState.isOn)
  //       {
  //         Serial.println("on");
  //       }
  //       else
  //       {
  //         Serial.println("off");
  //       }
  //     }
  //     if(result.remainingZones == 0){break;}
  //   }
  //   digitalWrite(LED_BUILTIN,     LOW);
  //   Serial.println("------");
  // }
  while(Serial.available())
  {
    char c = Serial.read();
    input_text[ptr] = c;
    ptr++;
    if(c == 0x0A)
    {
      heaterController.processCommand(input_text);
      ptr = 0;
      memset(input_text, 0, sizeof(input_text));
    }
  }
}
