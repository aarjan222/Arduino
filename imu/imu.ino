#include <Wire.h>
#include "Kalman.h"

Kalman kalmanX;  // Create the Kalman instances
Kalman kalmanY;

/* IMU Data */
long accX, accY, accZ;
long gyroX, gyroY, gyroZ;

// float gForceX, gForceY, gForceZ;
float gyroXrate, gyroYrate, gyroZrate;

double gyroXangle, gyroYangle;  // Angle calculate using the gyro only
double compAngleX, compAngleY;  // Calculated angle using a complementary filter
double kalAngleX, kalAngleY;    // Calculated angle using a Kalman filter
uint32_t timer;

double dt, roll, pitch;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  setupMPU();

  Wire.beginTransmission(0b1101000);  //communication with I2C Module and MPU6050
  Wire.write(0x3b);                   //access register 3b for accel readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000, 6);  //Request Accel Registers (3B-40)
  while (Wire.available() < 6)
    ;
  accX = Wire.read() << 8 | Wire.read();  //Store first two bytes into accX
  accY = Wire.read() << 8 | Wire.read();  //Store middle two bytes into accY
  accZ = Wire.read() << 8 | Wire.read();  //Store last two bytes into accZ

  roll = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;  //x axis
  pitch = atan2(-accX, accZ) * RAD_TO_DEG;                           //y axis

  kalmanX.setAngle(roll);
  kalmanY.setAngle(pitch);
  gyroXangle = roll;
  gyroYangle = pitch;
  compAngleX = roll;
  compAngleY = pitch;
  timer = micros();
}

void loop() {
  recordAccelRegisters();
  recordGyroRegisters();
  printData();
  // delay(2000);
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
  Wire.write(0b00000000);  //setting accel to +/- 2g
  Wire.endTransmission();
}
void recordAccelRegisters() {
  Wire.beginTransmission(0b1101000);  //communication with I2C Module and MPU6050
  Wire.write(0x3b);                   //access register 3b for accel readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000, 6);  //Request Accel Registers (3B-40)
  while (Wire.available() < 6)
    ;
  accX = Wire.read() << 8 | Wire.read();  //Store first two bytes into accX
  accY = Wire.read() << 8 | Wire.read();  //Store middle two bytes into accY
  accZ = Wire.read() << 8 | Wire.read();  //Store last two bytes into accZ
  // processAccelData();
}
// void processAccelData() {
//   gForceX = accX / 16384.0 * 9.81;
//   gForceY = accY / 16384.0 * 9.81;
//   gForceZ = accZ / 16384.0 * 9.81;
// }
void recordGyroRegisters() {
  Wire.beginTransmission(0b1101000);  //communication with I2C Module and MPU6050
  Wire.write(0x43);                   //access register 3b for gyro readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000, 6);  //Request Accel Registers (3B-40)
  while (Wire.available() < 6)
    ;
  gyroX = Wire.read() << 8 | Wire.read();  //Store first two bytes into accX
  gyroY = Wire.read() << 8 | Wire.read();  //Store middle two bytes into accY
  gyroZ = Wire.read() << 8 | Wire.read();  //Store last two bytes into accZ

  dt = (double)(micros() - timer) / 1000000;  // Calculate delta time
  timer = micros();

  roll = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
  pitch = atan2(-accX, accZ) * RAD_TO_DEG;
  processGyroData();
}
void processGyroData() {
  gyroXrate = gyroX / 131.0;  // Convert to deg/s
  gyroYrate = gyroY / 131.0;
  gyroZrate = gyroZ / 131.0;

  // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
  if ((pitch < -90 && kalAngleY > 90) || (pitch > 90 && kalAngleY < -90)) {
    kalmanY.setAngle(pitch);
    compAngleY = pitch;
    kalAngleY = pitch;
    gyroYangle = pitch;
  } else {
    kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt);  // Calculate the angle using a Kalman filter
  }
  if (abs(kalAngleY) > 90) {
    gyroXrate = -gyroXrate;                             // Invert rate, so it fits the restriced accelerometer reading
    kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt);  // Calculate the angle using a Kalman filter
  }
  gyroXangle += gyroXrate * dt;  // Calculate gyro angle without any filter
  gyroYangle += gyroYrate * dt;

  //gyroXangle += kalmanX.getRate() * dt; // Calculate gyro angle using the unbiased rate
  //gyroYangle += kalmanY.getRate() * dt;

  compAngleX = 0.93 * (compAngleX + gyroXrate * dt) + 0.07 * roll;  // Calculate the angle using a Complimentary filter
  compAngleY = 0.93 * (compAngleY + gyroYrate * dt) + 0.07 * pitch;

  // Reset the gyro angle when it has drifted too much
  if (gyroXangle < -180 || gyroXangle > 180) {
    gyroXangle = kalAngleX;
  }
  if (gyroYangle < -180 || gyroYangle > 180) {
    gyroYangle = kalAngleY;
    map(1, 2, 2, 2, 2);
  }
}

void printData() {
  Serial.print("\nGX: ");
  Serial.print(gyroXrate);
  Serial.print("\tGY: ");
  Serial.print(gyroYrate);
  Serial.print("\tGZ: ");
  Serial.print(gyroZrate);

  Serial.print("\t");

  // Serial.print("AX: ");
  // Serial.print(gForceX);
  // Serial.print("\tAY: ");
  // Serial.print(gForceY);
  // Serial.print("\tAZ: ");
  // Serial.print(gForceZ);

  // Serial.print("\t");

  // Serial.print("accX");
  // Serial.print(accX);
  // Serial.print("\t");
  // Serial.print("accY");
  // Serial.print(accY);
  // Serial.print("\t");
  // Serial.print("accZ");
  // Serial.print(accZ);
  // Serial.print("\t");

  Serial.print("roll");
  Serial.print(roll);
  Serial.print("\t");
  Serial.print("gyroXangle");
  Serial.print(gyroXangle);
  Serial.print("\t");
  Serial.print("compAngleX");
  Serial.print(compAngleX);
  Serial.print("\t");
  Serial.print("kalAngleX");
  Serial.print(kalAngleX);
  Serial.print("\t");

  Serial.print("\t");
  Serial.print("pitch");
  Serial.print(pitch);
  Serial.print("\t");
  Serial.print("gyroYangle");
  Serial.print(gyroYangle);
  Serial.print("\t");
  Serial.print("compAngleY");
  Serial.print(compAngleY);
  Serial.print("\t");
  Serial.print("kalAngleY");
  Serial.print(kalAngleY);
  Serial.print("\t");
}