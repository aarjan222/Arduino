volatile long temp, counter = 0; //This variable will increase or decrease depending on the rotation of encoder
uint8_t dir =0;
//BLACK=2=OUTA
//WHITE=3=OUTB
//ORANGE=NO=OUTZ

void setup() {
  Serial.begin (9600);

  pinMode(2, INPUT_PULLUP); // internal pullup input pin 2

  pinMode(3, INPUT_PULLUP); // internal pullup input pin 3

  //attachInterrupt(digitalPinToInterrupt(pin), ISR, mode) (recommended)
  //attachInterrupt(interrupt, ISR, mode) (not recommended)

  //Setting up interrupt
  //A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.

  attachInterrupt(digitalPinToInterrupt(2), ai0, RISING);

  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.
  attachInterrupt(digitalPinToInterrupt(3), ai1, RISING);
}

void loop() {
  Serial.print(counter    );
  if(dir == 1)
  {
    Serial.print("   CW   ");
  }
  else
  {
    Serial.print("   ACW   ");
  }
  Serial.println();
  // Send the value of counter
  if ( counter != temp ) {
    Serial.println (counter);
    temp = counter;
  }
}

void ai0() {
  //FOR CLOCKWISE
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
///  check = digitalRead(3);
  if (digitalRead(3) == LOW) {
    dir=1; //dir=1 indicates clockwise
  } else {
    dir=0; //dir=0 indicates anticlockwise
  }
  if(digitalRead(3) == LOW && digitalRead(2) == HIGH)
  {
    counter++;
  }
  else{
    counter--;
  }
  
}

void ai1() {
  //FOR ANTI-CLOCKWISE
  // ai1 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if (digitalRead(2) == LOW) {
    dir = 0; //dir=0 indicates anticlockwise
  } else {
    dir = 1; //dir=1 indicates clockwise
  }
   if(digitalRead(2) == LOW && digitalRead(3) == HIGH)
  {
    counter--;
  }
  else{
    counter++;
  }
}
//  if B != A, clockwise.
//if B = A, counterclockwise.
