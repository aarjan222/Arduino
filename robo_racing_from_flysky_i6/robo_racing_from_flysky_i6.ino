unsigned long rc_pulse1 = 0, rc_pulse2 = 0;
float speed = 0.0;
void setup() {
  // put your setup code here, to run once:
  // take input from the receiver
  pinMode(3, INPUT);
  pinMode(5, INPUT);
  // direction pins
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  // pwm pins
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  // initially stop motors
  set_speed(0.0);
  Serial.begin(9600);
}

void loop() {
  // delay(250);
  rc_pulse1 = pulseIn(5, HIGH, 25000);
  rc_pulse2 = pulseIn(3, HIGH, 25000);
  // delay(250);

  // check forward or backward
  if(rc_pulse1>2000){
    rc_pulse1=2000;
  }
  if(rc_pulse1<1000){
    rc_pulse1=1000;
  }
  if (rc_pulse1 > 1500) {

    speed = map(rc_pulse1, 1500, 2000, 0, 255);
    Serial.print("forward\t");
    set_forward_direction();
  } else if (rc_pulse1 < 1500) {
    speed = map(rc_pulse1, 1500, 1000, 0, 255);
    Serial.print("backward\t");
    set_backward_direction();
  }

  // check left or right
  if (rc_pulse2 > 1500) {
    Serial.println("right\t");
    right();
  } else if (rc_pulse2 < 1500) {
    Serial.println("left\t");
    left();
  }
  set_speed(speed);
  Serial.print(speed);
  // Serial.print("\t");
  // Serial.println(rc_pulse2);
}


void set_speed(float speed) {
  analogWrite(10, speed);
  analogWrite(11, speed);
}

void set_forward_direction() {
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);
}

void set_backward_direction() {
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
}

void left() {
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
}

void right() {
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);
}