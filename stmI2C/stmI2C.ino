#include "Wire.h"

#define Device_ID 0x33
int counter = 0;


void setup() {
  Wire.begin(Device_ID);
  Wire.onRequest(requestEvent);
  Serial.begin(9600);
}

void loop() {
}

void requestEvent() {
  counter++;
  String data = "Hello Aarjan Budathoki Ardu: " + String(counter);
  // send upto 32bytes only
  Serial.println(data);
  Wire.write(data.c_str());
}