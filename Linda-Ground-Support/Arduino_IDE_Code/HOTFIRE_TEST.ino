//Assuming Load Cell Calibration

#include "HX711.h"
#define LOAD_CELL_DOUT 4 // Pin 4 on Arduino Uno
#define LOAD_CELL_CLK 5 // SCK Pin on arduino Uno
HX711 scale;

const float calibration_factor = 2230 ; //Arbritatry number for now, get this numbeer from calibration sketch = READING / WEIGHT
float units = 0;
void setup() {
  Serial.begin(9600);
  scale.begin(LOAD_CELL_DOUT, LOAD_CELL_CLK);
  scale.set_scale(calibration_factor);
}

void loop() {
  Serial.println("One single Reading:");
  Serial.println(scale.get_units(), 1);
  Serial.println(" To get an average of last 10 raw readings ");
  Serial.println(scale.get_units(10),5);
}

