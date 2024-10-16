#include <SPI.h>
#include "HeaterController.h"


HeaterController heaterController;

void setup() {
  SPI.begin();
  Serial.begin(9600);
  delay(1500);
  Serial.println("---reset---");
  heaterController.addZone("Mass_spec", 0, 0, 0, 0, 0);
  heaterController.addZone("Analyte", 0, 0, 0, 0, 0);
  heaterController.addZone("Sampler", 0, 0, 0, 0, 0);
  heaterController.addZone("Capillary", 0, 0, 0, 0, 0);

  pinMode(38, OUTPUT);
  digitalWrite(38, HIGH);

  pinMode(40, OUTPUT);
  digitalWrite(40, HIGH);

  pinMode(42, OUTPUT);
  digitalWrite(42, HIGH);

  pinMode(44, OUTPUT);
  digitalWrite(44, HIGH);

}

char input_text[256];
int ptr;
void loop() {
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
