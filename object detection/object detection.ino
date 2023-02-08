#include <Servo.h>

Servo myServo;

int pos = 0, temp_pos = 0;
long duration;
int distance;
int Trig_PIN = 5;
int Echo_PIN = 4;
bool rotate = true;
bool dir = true;

void setup() {
  pinMode(Trig_PIN, OUTPUT);
  pinMode(Echo_PIN, INPUT);
  Serial.begin(9600);
  myServo.attach(9);
}

void loop() {
  if (dir == true && rotate == true) {
    for (; pos <= 180; pos += 5) {
      myServo.write(pos);
      digitalWrite(Trig_PIN, LOW);
      delayMicroseconds(2);
      digitalWrite(Trig_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(Trig_PIN, LOW);
      duration = pulseIn(Echo_PIN, HIGH);
      distance = duration * 0.034 / 2;
      // Serial.print("Distance: ");
      // Serial.print(distance);
      // Serial.println(" cm");
      if (distance <= 10) {
        Serial.print("clockwise stop at ");
        Serial.println(pos);
        myServo.write(pos);
        rotate = false;
        dir = true;
        break;
      }
      delay(100);
    }
    rotate = true;
  }

  if (pos >= 180) {
    dir = false;
  }

  if (dir == false && rotate == true) {
    for (; pos >= 0; pos -= 5) {
      myServo.write(pos);
      digitalWrite(Trig_PIN, LOW);
      delayMicroseconds(2);
      digitalWrite(Trig_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(Trig_PIN, LOW);
      duration = pulseIn(Echo_PIN, HIGH);
      distance = duration * 0.034 / 2;
      if (distance <= 10) {
        Serial.print("anticlockwise stop at ");
        Serial.println(pos);
        myServo.write(pos);
        rotate = false;
        break;
      }
      delay(100);
    }
    rotate = true;
  }
  if (pos <= 0) {
    dir = true;
  }
}