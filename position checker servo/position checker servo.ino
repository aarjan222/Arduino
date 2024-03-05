#include <Servo.h>
Servo myServo;

void setup() {
  // put your setup code here, to run once:
  myServo.attach(4);
  myServo.write(85);
  delay(1000);
  myServo.write(60);
  delay(1000);
  myServo.write(110);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:

  // for (int i = 120; i >= 50; i--) {
  //   myServo.write(i);
  //   delay(10);
  // }

  // delay(2000);
  // // myServo.write(90);


  // for (int i = 50; i <= 120; i++) {
  //   myServo.write(i);
  //   delay(10);
  // }
  // delay(2000);
  myServo.write(85);
  delay(5000);
  myServo.write(60);
  delay(5000);
  myServo.write(85);
  delay(1000);
  myServo.write(110);
  delay(5000);
}
