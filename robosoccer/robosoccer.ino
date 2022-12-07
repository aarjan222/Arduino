#include <SoftwareSerial.h>

const int en1 = 5, en2 = 6, in1 = 4, in2 = 7, in3 = 8, in4 = 9, statePin = A0;
char c = ' ';
SoftwareSerial myBT(0, 1);  //0 as rx and 1 ax tx

void setup() {
  Serial.begin(9600);
  myBT.begin(115200);
  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(statePin, INPUT);
}

void loop() {
  digitalWrite(en1, HIGH);
  digitalWrite(en2, HIGH);
  if (analogRead(statePin) == 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }
  else
  {
    if (myBT.available() > 0) {
      c = myBT.read();
      switch (c) {
        case 'F':
          digitalWrite(in1, HIGH);
          digitalWrite(in2, LOW);
          digitalWrite(in3, HIGH);
          digitalWrite(in4, LOW);
          break;

        case 'L':
          digitalWrite(in1, LOW);
          digitalWrite(in2, HIGH);
          digitalWrite(in3, HIGH);
          digitalWrite(in4, LOW);
          break;

        case 'R':
          digitalWrite(in1, HIGH);
          digitalWrite(in2, LOW);
          digitalWrite(in3, LOW);
          digitalWrite(in4, HIGH);
          break;

        case 'B':
          digitalWrite(in1, LOW);
          digitalWrite(in2, HIGH);
          digitalWrite(in3, LOW);
          digitalWrite(in4, HIGH);
          break;

        case 'S':
          digitalWrite(in1, LOW);
          digitalWrite(in2, LOW);
          digitalWrite(in3, LOW);
          digitalWrite(in4, LOW);
          break;
      }
    }
  }
}
