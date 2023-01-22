#include <Servo.h>
Servo myservo;

void setup() {
  myservo.attach(9);
}

void loop() {
  myservo.write(170);
  delay(2000); 

  myservo.write(10);
  delay(2000);
  map()
}