#include <Wire.h>

long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;

long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;
void setup() {
  Serial.begin(115200);
  Wire.begin();
  setupMPU();
}

void loop() {
  recordAccelRegisters();
  recordGyroRegisters();
  printData();
  delay(2000);
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
  accelX = Wire.read() << 8 | Wire.read();  //Store first two bytes into accelX
  accelY = Wire.read() << 8 | Wire.read();  //Store middle two bytes into accelY
  accelZ = Wire.read() << 8 | Wire.read();  //Store last two bytes into accelZ
  processAccelData();
}
void processAccelData() {
  gForceX = accelX / 16384.0 * 9.81;
  gForceY = accelY / 16384.0 * 9.81;
  gForceZ = accelZ / 16384.0 * 9.81;
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
  rotX = gyroX / 131.0;
  rotY = gyroY / 131.0;
  rotZ = gyroZ / 131.0;
}
void printData() {
  Serial.print("\nGX: ");
  Serial.print(rotX);
  Serial.print("\tGY: ");
  Serial.print(rotY);
  Serial.print("\tGZ: ");
  Serial.print(rotZ);

  Serial.print("\n\nAX: ");
  Serial.print(gForceX + 0.42);
  Serial.print("\tAY: ");
  Serial.print(gForceY - 0.27);
  Serial.print("\tAZ: ");
  Serial.print(gForceZ);
}