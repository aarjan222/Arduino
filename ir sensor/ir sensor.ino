uint16_t counter = 0;
unsigned long previousVal = 0;
unsigned long previousCount = 0;
uint16_t diffVal = 1000;
float rpm;
bool black_detected = false;
void setup() {
  pinMode(2, INPUT_PULLUP);  // internal pullup input pin 2
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), ai0, RISING);
  previousVal = millis();
  Serial.println("hello");
}

void loop() {
  // Serial.print(counter);
  if (millis() - previousVal >= diffVal) {
    rpm = (counter - previousCount) * 60 / 5;
    previousCount = counter;
    previousVal = millis();
    // y
    Serial.println(rpm);
  }
}

void ai0() {
  // if (digitalRead(2) == LOW) {
  //   // Serial.println("white object detected");
  // } else {
  //   // Serial.println("black object not detected");
  //   black_detected = true;
  // }

  // if (black_detected) {
  counter++;
  //   black_detected = false;
  // }
}