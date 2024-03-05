const int motor1PWM = 3;
const int motor1Dir = 4;

const int motor2PWM = 6;
const int motor2Dir = 7;


void setup() {
  pinMode(motor1PWM, OUTPUT);
  pinMode(motor1Dir, OUTPUT);

  pinMode(motor2PWM, OUTPUT);
  pinMode(motor2Dir, OUTPUT);

  analogWrite(motor1PWM, 0);
  digitalWrite(motor1Dir, LOW);

  analogWrite(motor2PWM, 0);
  digitalWrite(motor2Dir, LOW);

  Serial.begin(9600);
}

void loop() {
  if (Serial.available() >= 24) {
    // Read 24 bytes as three float values for linear and angular velocities
    float linear_x, linear_y, linear_z, angular_x, angular_y, angular_z;
    Serial.readBytes((char*)&linear_x, sizeof(float));
    Serial.readBytes((char*)&linear_y, sizeof(float));
    Serial.readBytes((char*)&linear_z, sizeof(float));
    Serial.readBytes((char*)&angular_x, sizeof(float));
    Serial.readBytes((char*)&angular_y, sizeof(float));
    Serial.readBytes((char*)&angular_z, sizeof(float));

    int motorSpeed = static_cast<int>(linear_x * 255);

    Serial.println("Linear");
    Serial.println(linear_x);
    Serial.println(linear_y);
    Serial.println(linear_z);
    Serial.println("Angular");
    Serial.println(angular_x);
    Serial.println(angular_y);
    Serial.println(angular_z);

    // setMotorSpeed(motor1PWM, motor1Dir, motorSpeed);
    // setMotorSpeed(motor2PWM, motor2Dir, motorSpeed);
  }
}

void setMotorSpeed(int pwmPin, int dir1Pin, int speed) {
  if (speed >= 0) {
    digitalWrite(dir1Pin, HIGH);
  } else {
    digitalWrite(dir1Pin, LOW);
    speed = -speed;
  }

  analogWrite(pwmPin, speed);
}
