#include <SPI.h>
#include "HeaterController.h"
#include "PinAssignment.h"

HeaterController heaterController;
PinAssignment pinAssignment;

static volatile int something;
void setup() {
  pinMode(MISO, INPUT);
  pinMode(SCK, OUTPUT);
  pinMode(SS, OUTPUT);
  pinMode(MOSI, OUTPUT);
  digitalWrite(SS, HIGH);
  digitalWrite(SCK, HIGH);

  pinMode(A0, INPUT);

  // while(1)
  // {
  //   digitalWrite(pinAssignment.HeaterList.CapillaryHeater.CsPin, HIGH);
  //   digitalWrite(SCK, HIGH);
  //   delay(5);
  //   digitalWrite(pinAssignment.HeaterList.CapillaryHeater.CsPin, LOW);
  //   digitalWrite(SCK, LOW);
  //   delay(5);
  // }

  Serial.begin(9600);
  while(!Serial){delay(1);}
  Serial.println("---reset---");
  heaterController.addZone("mass_spec", pinAssignment.HeaterList.MassSpecHeater.CsPin, pinAssignment.HeaterList.MassSpecHeater.OutPin, 0, 0, 0);
  heaterController.addZone("analyte",   pinAssignment.HeaterList.AnalyteHeater.CsPin, pinAssignment.HeaterList.AnalyteHeater.OutPin, 0, 0, 0);
  heaterController.addZone("sampler",   pinAssignment.HeaterList.SamplerHeater.CsPin, pinAssignment.HeaterList.SamplerHeater.OutPin, 0, 0, 0);
  heaterController.addZone("capillary", pinAssignment.HeaterList.CapillaryHeater.CsPin, pinAssignment.HeaterList.CapillaryHeater.OutPin, 0, 0, 0);

  pinMode(pinAssignment.HeaterList.MassSpecHeater.CsPin,  OUTPUT);
  pinMode(pinAssignment.HeaterList.AnalyteHeater.CsPin,   OUTPUT);
  pinMode(pinAssignment.HeaterList.SamplerHeater.CsPin,   OUTPUT);

  pinMode(pinAssignment.HeaterList.MassSpecHeater.OutPin,   OUTPUT);
  pinMode(pinAssignment.HeaterList.AnalyteHeater.OutPin,    OUTPUT);
  pinMode(pinAssignment.HeaterList.SamplerHeater.OutPin,    OUTPUT);
  pinMode(pinAssignment.HeaterList.CapillaryHeater.OutPin,  OUTPUT);

  digitalWrite(pinAssignment.HeaterList.SamplerHeater.OutPin,     LOW);
  digitalWrite(pinAssignment.HeaterList.MassSpecHeater.OutPin,    LOW);
  digitalWrite(pinAssignment.HeaterList.AnalyteHeater.OutPin,     LOW);
  digitalWrite(pinAssignment.HeaterList.CapillaryHeater.OutPin,   LOW);

  digitalWrite(pinAssignment.HeaterList.SamplerHeater.OutPin,     HIGH);
  digitalWrite(pinAssignment.HeaterList.MassSpecHeater.OutPin,    HIGH);
  digitalWrite(pinAssignment.HeaterList.AnalyteHeater.OutPin,     HIGH);
  digitalWrite(pinAssignment.HeaterList.CapillaryHeater.OutPin,   HIGH);

  delay(500);

  digitalWrite(pinAssignment.HeaterList.MassSpecHeater.CsPin,   HIGH);
  digitalWrite(pinAssignment.HeaterList.AnalyteHeater.CsPin,    HIGH);
  digitalWrite(pinAssignment.HeaterList.SamplerHeater.CsPin,    HIGH);
  digitalWrite(pinAssignment.HeaterList.CapillaryHeater.CsPin,  HIGH);


}

char input_text[256];
int ptr;
void loop() {
  delay(500);
  if(heaterController.isReady())
  {
    Serial.println("------");
    while(1)
    {
      //digitalWrite(pinAssignment.HeaterList.SamplerHeater.OutPin,     HIGH);
      HeaterCtrl_ret_t result = heaterController.update();
      Serial.print(result.zoneName); Serial.print(" "); Serial.print(result.zoneState.TcState.ext_celcius);
      Serial.print(" / ");Serial.print(result.zoneState.setPoint);Serial.print(" / ");
      if(result.zoneState.TcState.error)
      {
        Serial.println("error");
      }
      else
      {
        if(result.zoneState.isOn)
        {
          Serial.println("on");
        }
        else
        {
          Serial.println("off");
        }
      }
      if(result.remainingZones == 0){break;}
    }
    //digitalWrite(pinAssignment.HeaterList.SamplerHeater.OutPin,     LOW);
    Serial.println("------");
  }
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
