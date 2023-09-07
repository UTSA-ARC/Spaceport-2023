const int thermocouple_input = A1;	/* AD595 O/P pin */

void setup() {
  Serial.begin(9600);	/* Define baud rate for serial communication */
}

void loop() {
  int adc_val;
  float temperature;
  adc_val = analogRead(thermocouple_input);
  temperature = ( ((adc_val * 4.88) - 0.0027 ) / 10.0 );
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.print("\n\n");
  delay(1000);
}