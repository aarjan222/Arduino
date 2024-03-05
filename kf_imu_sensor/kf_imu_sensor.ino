#include <Wire.h>
#include <math.h>
#include "ArrbotMonitor.h"

long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;

long gyroX, gyroY, gyroZ;
float RateRoll, RatePitch, RateYaw;  //roll in x -- pitch in y -- yaw in z

float RateCallibrationRoll, RateCallibrationPitch, RateCallibrationYaw;
int RateCallibrationNumber;

uint32_t loopTimer;

double AngleRoll = 0, AnglePitch = 0, AngleYaw = 0;

//define the predicted angles
float KalmanAngleRoll = 0, KalmanAnglePitch = 0;  //initial guess for the angle is 0 starts from ground
//define the predicted uncertainity
float KalmanUncertainityAngleRoll = 2 * 2, KalmanUncertainityAnglePitch = 2 * 2;

//Initialize the output of the filter
float Kalman1DOutput[] = { 0, 0 };  // angle prediction and uncertainity of the prediciton
//both updated during each iteration

// KalmanInput = rotation rate measurement
//KalmanMeasurement = accelerometer angle measurement
// KalmanState = the angle calculated with Kalman Filter
void kalman_1d(float KalmanState, float KalmanUncertainity, float KalmanInput, float KalmanMeasurement) {
  KalmanState = KalmanState + 0.004 * KalmanInput;                       // predict the current state of the system
  KalmanUncertainity = KalmanUncertainity + 0.004 * 0.004 * 4 * 4;       // Calculate the uncertainity on the prediction
  float KalmanGain = KalmanUncertainity / (KalmanUncertainity + 3 * 3);  // Calculate the Kalman Gain from the uncertainites on the predictions  and measurements

  KalmanState = KalmanState + KalmanGain * (KalmanMeasurement - KalmanState);  // Update the predicted state of the system with the measurement of the state through the Kalman Gain

  KalmanUncertainity = (1 - KalmanGain) * KalmanUncertainity;  // Updates the uncertainity of the predicted state

  Kalman1DOutput[0] = KalmanState;
  Kalman1DOutput[1] = KalmanUncertainity;  // Kalman Filter Output
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  setupMPU();
}

void loop() {
  recordAccelRegisters();
  // Accelerometry Trignometry Angles
  AngleRoll = atan(accelY / sqrt(accelX * accelX + accelZ * accelZ)) * 180 / 3.142;  // convert radians to degrees
  AnglePitch = -atan(accelX / sqrt(accelY * accelY + accelZ * accelZ)) * 180 / 3.142;
  AngleYaw = atan(sqrt(accelX * accelX + accelY * accelY) / accelZ) * 180 / 3.142;
  // vibrations have huge impact on the angle
  recordGyroRegisters();

  printData();
  delay(50);
}
void setupMPU() {
  Wire.beginTransmission(0b1101000);  //I2C communication with mpu 6050
  Wire.write(0x6b);                   //acces register 6b power management
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1b);        //access register 1b Gyroscope config.
  Wire.write(0b00000000);  //setting gyro to full scale +/- 250deg/s
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1c);        //access register 1c Accelermeter Config.
  Wire.write(0b00010000);  //setting accel to +/- 8g
  Wire.endTransmission();
}
void recordAccelRegisters() {
  Wire.beginTransmission(0b1101000);  //communication with I2C Module and MPU6050
  Wire.write(0x3b);                   //access register 3b for accel readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000, 6);  //Request Accel Registers (3B-40)
  while (Wire.available() < 6)
    ;
  accelX = Wire.read() << 8 | Wire.read();  //Store first two bytes into accelX
  accelY = Wire.read() << 8 | Wire.read();  //Store middle two bytes into accelY
  accelZ = Wire.read() << 8 | Wire.read();  //Store last two bytes into accelZ
  processAccelData();
}
void processAccelData() {
  gForceX = accelX / (float)4096;
  gForceY = accelY / (float)4096;
  gForceZ = accelZ / (float)4096;
}
void recordGyroRegisters() {
  Wire.beginTransmission(0b1101000);  //communication with I2C Module and MPU6050
  Wire.write(0x43);                   //access register 3b for gyro readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000, 6);  //Request Accel Registers (3B-40)
  while (Wire.available() < 6)
    ;
  gyroX = Wire.read() << 8 | Wire.read();  //Store first two bytes into accelX
  gyroY = Wire.read() << 8 | Wire.read();  //Store middle two bytes into accelY
  gyroZ = Wire.read() << 8 | Wire.read();  //Store last two bytes into accelZ
  processGyroData();
}
void processGyroData() {
  RateRoll = (float)gyroX / 65.5;
  RateYaw = (float)gyroY / 65.5;
  RatePitch = (float)gyroZ / 65.5;

  //2s loop takes 2000 datas and take avg values if sensor is in rest condition
  for (RateCallibrationNumber; RateCallibrationNumber < 2000; RateCallibrationNumber++) {
    RateCallibrationRoll += RateRoll;
    RateCallibrationPitch += RatePitch;
    RateCallibrationYaw += RateYaw;
    delay(1);  // 1 millisecond
  }

  RateCallibrationRoll /= 2000;
  RateCallibrationPitch /= 2000;
  RateCallibrationYaw /= 2000;

  RateRoll -= RateCallibrationRoll;
  RatePitch -= RateCallibrationPitch;
  RateYaw -= RateCallibrationYaw;

  // implement kalman filter to the angles of roll, pitch and yaw
  kalman_1d(KalmanAngleRoll, KalmanUncertainityAngleRoll, RateRoll, AngleRoll);
  KalmanAngleRoll = Kalman1DOutput[0];
  KalmanUncertainityAngleRoll = Kalman1DOutput[1];

  kalman_1d(KalmanAnglePitch, KalmanUncertainityAnglePitch, RatePitch, AnglePitch);
  KalmanAnglePitch = Kalman1DOutput[0];
  KalmanUncertainityAnglePitch = Kalman1DOutput[1];


  loopTimer = micros();
}
void printData() {
  // DISPLAY(AngleRoll);
  // MONITOR(AngleRoll);
  // MONITOR2("ANGLEROLL", AnglePitch);
  // MONITOR2("ANGLEYAW", AngleYaw);
  // MONITOR_ENDL();
  Serial.print(KalmanAngleRoll);
  Serial.print("\t");
  Serial.println(AngleRoll);
  // Serial.println((KalmanAnglePitch));
  // DISPLAY(KalmanAngleRoll);
  // MONITOR(KalmanAngleRoll);
  // MONITOR2("KalmanAnglePitch", KalmanAnglePitch);
  // Serial.println(AngleRoll);
  while (micros() - loopTimer < 4000)
    ; // wait for 4 seconds
  loopTimer = micros();
}