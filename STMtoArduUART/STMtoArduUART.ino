#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);  // RX, TX

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }


  Serial.println("Goodnight moon!");
  mySerial.begin(115200);
}

void loop() {
  if (mySerial.available()) {
    char ch = mySerial.read();
    Serial.println(ch);
    // Serial.write(mySerial.read());
  }
}
