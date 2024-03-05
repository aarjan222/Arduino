int enc1 = 2, enc2 = 3;
int count = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode(enc1, INPUT_PULLUP);
  pinMode(enc2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(enc1), encodercount1, RISING);
  attachInterrupt(digitalPinToInterrupt(enc2), encodercount2, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(count);
}


void encodercount1() {
  if (digitalRead(enc2) && !digitalRead(enc1)) {
    count++;
  } else {
    count--;
  }
  if (!digitalRead(enc2) && digitalRead(enc1)) {
    count++;
  } else {
    count--;
  }
}

void encodercount2() {
  if (!digitalRead(enc1) && digitalRead(enc2)) {
    count--;
  } else {
    count++;
  }
  if (digitalRead(enc1) && !digitalRead(enc2)) {
    count--;
  } else {
    count++;
  }
}