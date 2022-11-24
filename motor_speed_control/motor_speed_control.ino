const int motor_speed = 25 * 255 / 100;

const int pwm1 = 8;
const int pwm2 = 9;
const int dir1 = 10;
const int dir2 = 11;
const int check1 = 12;
const int check2 = 3;
void setup() 
{
  pinMode(pwm1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(dir2,OUTPUT);
  pinMode(check1, INPUT);
  pinMode(check2, INPUT);
}

void loop() {

  if (digitalRead(check1) == HIGH)
  {
  digitalWrite(dir1, 1);
  analogWrite(pwm1, motor_speed);
  }

  if (digitalRead(check2) == HIGH)
  {
  digitalWrite(dir1, 0);
  analogWrite(pwm1, motor_speed);
  }

}
