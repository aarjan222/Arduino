#include <SoftwareSerial.h>

// SoftwareSerial myBT(0, 1);

void setup() {
  Serial.begin(115200);
  Serial3.begin(115200);
}
void loop() {
  Serial.write('a');
  // if (myBT.available()) {
    
  // }
  delay(1000);
}