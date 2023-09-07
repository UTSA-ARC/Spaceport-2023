#include <Wire.h>
const int pressureInput = A0;
const int pressureZero = 102.4; //Analog reading of pressure transducer at 0psi  x/1024 = 0.5v / 5v ---- datasheet, @0.5v pressure reads 0psi
const int pressureMax = 921.6; //Analog reading of pressure transducer at 100psi  x/1024 = 4.5v / 5v ---- datasheet, @4.5v pressure reads 100psi
//Convert from analog to voltage 
const int pressureTransduancerMaxPSI = 100; // PSI value of transfucer being used

float pressureValue  = 0 ;


void setup() {
  Serial.begin(9600);
}

void loop() {
  pressureValue = analogRead(pressureInput);
  pressureValue = ((pressureValue - pressureZero) * pressureTransduancerMaxPSI) / (pressureMax - pressureZero); //conversion from analog to voltage
  //can confirm this value on arduino is with multimeter, go between input of analog pin and to ground pin.  should give a reading 

  Serial.print(pressureValue , 3) ; //three decimal places
  Serial.print("psi");
  delay(1000);
  
}

