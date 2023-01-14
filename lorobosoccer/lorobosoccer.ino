#include "crc_hash.h"
#include "joystick.h"
#include "array.h"
#include <stdint.h>
#include <SoftwareSerial.h>
#include <math.h>

#define DEBUG_MODE

#define JOYSTICK_START_BYTE (START_BYTE)
#define NUM_JOYSTICK_BYTES (8)

static CRC_Hash gJoyStick_CRC;

static uint8_t gRx2Data;
static uint16_t gRx2_Data_num = 0;
uint8_t gJoy_Data_Arr[NUM_JOYSTICK_BYTES];
bool isHashMatched = false;
static bool gStart_Byte_Rx2 = false;

struct JoyStick_Data jData;

const int pwm1 = 3, pwm2 = 6, dir1 = 2, dir2 = 5;
char c = ' ';

SoftwareSerial myBT(0, 1);  // 0 as rx and 1 ax tx

void setup() {
  Serial.begin(115200);
  gJoyStick_CRC.begin(7);
  myBT.begin(115200);
  Serial.println("ready");
  pinMode(pwm1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
  // pinMode(statePin, INPUT);
}

void loop() {
  while (myBT.available()) {
    gRx2Data = myBT.read();
    if (!gStart_Byte_Rx2) {
      if (gRx2Data == JOYSTICK_START_BYTE) {
        gStart_Byte_Rx2 = true;
      }
    } else {
      if (gRx2_Data_num < NUM_JOYSTICK_BYTES) {
        gJoy_Data_Arr[gRx2_Data_num] = gRx2Data;
        ++gRx2_Data_num;
      } else {
        uint8_t rem = gRx2Data;
        gStart_Byte_Rx2 = false;
        gRx2_Data_num = 0;
        uint8_t hash = gJoyStick_CRC.get_Hash(gJoy_Data_Arr, NUM_JOYSTICK_BYTES);
        if (hash == rem) {
          isHashMatched = true;
        } else {
          isHashMatched = false;
        }
      }
    }
    if (isHashMatched) {
      // Serial.println("k ho");
      jData.button1 = gJoy_Data_Arr[0];
      jData.button2 = gJoy_Data_Arr[1];
      jData.lt = gJoy_Data_Arr[2];
      jData.rt = gJoy_Data_Arr[3];
      jData.l_hatx = gJoy_Data_Arr[4];
      jData.l_haty = gJoy_Data_Arr[5];
      jData.r_hatx = gJoy_Data_Arr[6];
      jData.r_haty = gJoy_Data_Arr[7];
#ifdef DEBUG_MODE
      // for (int i = 0; i < 8; i++) {
      //   Serial.print(gJoy_Data_Arr[i]);
      //   Serial.print("  ");
      // }
      // Serial.println();
#endif
      isHashMatched = false;
    }
    if (jData.l_haty > 10 && jData.l_hatx == 0) {
      forward();
#ifdef DEBUG_MODE
      Serial.println("forward");
#endif
    }
    if (jData.l_haty < -10 && jData.l_hatx == 0) {
      backward();
#ifdef DEBUG_MODE
      Serial.println("backward");
#endif
    }
    if (jData.l_hatx < -10 && jData.l_haty == 0) {
      left();
#ifdef DEBUG_MODE
      Serial.println("left");
#endif
    }
    if (jData.l_hatx > 10 && jData.l_haty == 0) {
      right();
#ifdef DEBUG_MODE
      Serial.println("right");
#endif
    }
    if (jData.l_haty < 5 && jData.l_haty > -5 && jData.lt < 5 && jData.l_hatx < 5 && jData.l_hatx > -5 && jData.rt < 5) {
      stop();
#ifdef DEBUG_MODE
      Serial.println("stop");
#endif
    }
    if (jData.rt > 20) {
      rotclock();
#ifdef DEBUG_MODE
      Serial.println("rotate_clockwise");
#endif
    }
    if (jData.lt > 20) {
      rotanticlock();
#ifdef DEBUG_MODE
      Serial.println("rotate_anticlockwise");
#endif
    }
  }
}
void forward() {
  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, LOW);
  analogWrite(pwm1, 255);
  analogWrite(pwm2, 255);
}

void backward() {
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, HIGH);
  analogWrite(pwm1, 255);
  analogWrite(pwm2, 255);
}
void right() {
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, HIGH);
  analogWrite(pwm1, 0);
  analogWrite(pwm2, 255);
}
void left() {
  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, LOW);
  analogWrite(pwm1, 255);
  analogWrite(pwm2, 0);
}
void rotclock() {
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, LOW);
  analogWrite(pwm1, 0);
  analogWrite(pwm2, 0);
}
void rotanticlock() {
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, LOW);
  analogWrite(pwm1, 0);
  analogWrite(pwm2, 0);
}
void stop() {
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, LOW);
  analogWrite(pwm1, 0);
  analogWrite(pwm2, 0);
}