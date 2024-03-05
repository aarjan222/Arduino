float xn1 = 0, yn1 = 0;
int k = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  //Test Signal
  float t = micros() / 1.0e6;
  float xn = sin(2 * PI * 2 * t) + 0.2 * sin(2 * PI * 50 * t);

  /*
While designing the low pass filter
attenuation characteristics
and 
delay caused by the filter
*/
  //Compute the filtered Signal
  //magnitude is slightly attenuated and there's a short delay
  float yn = 0.969 * yn1 + 0.0155 * xn + 0.0155 * xn1;

  delay(1);
  xn1 = xn;
  yn1 = yn;

  if (k % 3 == 0) {
    Serial.print(2 * xn);
    Serial.print(" ");
    Serial.println(2 * yn);
  }
  k = k + 1;
}
