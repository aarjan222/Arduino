unsigned long prevTime = 0;
void setup() {
  // put your setup code here, to run once:
  int i = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  // delay of 100ms
  if (millis() - prevTime >= 500) {
    Serial.prinln(i);
    i = i + 1;
    prevTime = millis();
  }
}
