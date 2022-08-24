// #define _WIRELESS_JOYSTICK_
#include <XBOXUSB.h>
#include <SoftwareSerial.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#ifdef _WIRELESS_JOYSTICK_
#include <XBOXRECV.h>
#endif

#include "crc_hash.h"

#define START_BYTE (0xA5)
#define BT_STATE_PIN 9

//#define DEBUG_MODE

USB Usb;
#ifndef _WIRELESS_JOYSTICK_
XBOXUSB Xbox(&Usb);
#else
XBOXRECV Xbox(&Usb);
#endif

#define _USE_SMALLEST_SIZE
#define JOUSTICK_SMALL_SIZE_NUM         (8)

// Data format
// 2 bytes for buttons

// First Byte: X Y A B Up Down Left Right
// Second Byte: START BACK XBOX

// A byte each for LT RT
// 2 bytes each for LeftHat {X,Y} and RightHat {X,Y}

// A total of 13 bytes including START_BYTE

uint8_t gJoyStick_Packet[13];

// Button's bit position in the byte
#define B_X             (7)
#define B_Y             (6)
#define B_A             (5)
#define B_B             (4)
#define B_UP            (3)
#define B_DOWN          (2)
#define B_LB            (1)
#define B_RB            (0)

#define B_START         (7)
#define B_BACK          (6)
#define B_XBOX          (5)
#define B_LEFT          (4)
#define B_RIGHT         (3)
#define B_L3            (2)
#define B_R3            (1)

// Bytes Positions in the packet

#define P_START         (0)
#define P_BUTTON1       (1)
#define P_BUTTON2       (2)
#define P_LT            (3)
#define P_RT            (4)
#define P_LHXH          (5)     // Left Hat X High
#define P_LHXL          (6)
#define P_LHYH          (7)
#define P_LHYL          (8)
#define P_RHXH          (9)
#define P_RHXL          (10)
#define P_RHYH          (11)
#define P_RHYL          (12)

#define _BV(x)          (1 << x)

void get_XBOX(uint8_t (&arr)[13]);

template <typename T, size_t N>
void fill_array(T (&arr)[N], T elem)
{
        for (uint8_t i = 0; i < N; ++i)
        {
                arr[i] = elem;
        }
}

CRC_Hash gJoyStick_CRC;
unsigned long gCurr_Time = 0;

void setup()
{
        Serial.begin(115200);
        Serial.println("Hello World!!");
        gJoyStick_CRC.begin(7);
        
        Serial3.begin(115200);
        Usb.Init();
        delay(500);
        fill_array(gJoyStick_Packet, (uint8_t)0);
        
        gCurr_Time = millis();
        pinMode(BT_STATE_PIN, INPUT);
}

bool is_Unrequired(uint8_t i)
{
        if (i == P_LHXH || i == P_LHYH
            || i == P_RHXH || i == P_RHYH) {
                    return true;
        }

        return false;
}
uint16_t prev = millis();
void loop()
{
        Usb.Task();
        bool state = digitalRead(BT_STATE_PIN);
        Serial.println(state);
        if (state == 0 )//bluetooth not connected
        {
            uint16_t counter = millis() - prev;
            Serial.print("Counter = ");
            Serial.println(counter);
           if (counter > 0 && counter <15000)
           {
              Xbox.setLedMode(ALTERNATING);
              Xbox.setRumbleOn(200,200);
              return ;
           }
           Xbox.setRumbleOn(0,0);
        }
        get_XBOX(gJoyStick_Packet); 
        uint32_t dt = millis();
#ifndef _USE_SMALLEST_SIZE
        Serial.print(dt);
        Serial.print(":  ");
        for (uint8_t i = 0; i < 13; ++i) {
                Serial.print(gJoyStick_Packet[i], HEX);
                Serial.print(" ");
        }
        Serial.println();
#else
//        Serial.print(dt);
//        Serial.print(":  ");
        if (millis() - gCurr_Time > 5) {
                gCurr_Time = millis();
                
                uint8_t j = 0;
                uint8_t joy_arr[JOUSTICK_SMALL_SIZE_NUM+1];
                for (uint8_t i = 0; i < 13; ++i) {
                        if (!is_Unrequired(i)) {
                                Serial3.write(gJoyStick_Packet[i]);
                                 Serial.print(gJoyStick_Packet[i], DEC);
                                 Serial.print(' ');
                                joy_arr[j++] = gJoyStick_Packet[i];
                        }
                }
                 Serial.println();
                uint8_t hash = gJoyStick_CRC.get_Hash(&joy_arr[1],JOUSTICK_SMALL_SIZE_NUM);
                // Serial.println(hash, DEC);
                Serial3.write(hash);
        }
        if (Serial3.available()) {
               char c = Serial3.read();
//               Seria/l.write(c);
        }
        
#endif
}

void get_XBOX(uint8_t (&arr)[13])
{
        uint8_t button_bytes[2] = { 0 };
        uint8_t left_trigger = 0;
        uint8_t right_trigger = 0;
        uint8_t l_hatx = 0;
        uint8_t r_hatx = 0;
        uint8_t l_haty = 0;
        uint8_t r_haty = 0;

        if (Xbox.Xbox360Connected)
        {

                if (Xbox.getButtonPress(L2))
                {
                        left_trigger = Xbox.getButtonPress(L2);
#ifdef DEBUG_MODE
                        Serial.print("LT: ");
                        Serial.println(left_trigger);
#endif // DEBUG_MODE
                }

                if (Xbox.getButtonPress(R2))
                {
                        right_trigger = Xbox.getButtonPress(R2);
#ifdef DEBUG_MODE
                        Serial.print("RT: ");
                        Serial.println(right_trigger);
#endif // DEBUG_MODE
                }

                if (Xbox.getButtonPress(L1))
                {
                        button_bytes[0] |= _BV(B_LB);
#ifdef DEBUG_MODE
                        Serial.print("LB");
                        Serial.println();
#endif // DEBUG_MODE
                }

                if (Xbox.getButtonPress(R1))
                {
                        button_bytes[0] |= _BV(B_RB);
#ifdef DEBUG_MODE
                        Serial.print("RB");
                        Serial.println();
#endif // DEBUG_MODE
                }

                if (Xbox.getButtonPress(X))
                {
                        button_bytes[0] |= _BV(B_X);
#ifdef DEBUG_MODE
                        Serial.print("X");
                        Serial.println();
#endif // DEBUG_MODE
                }

                if (Xbox.getButtonPress(Y))
                {
                        button_bytes[0] |= _BV(B_Y);
#ifdef DEBUG_MODE
                        Serial.print("Y");
                        Serial.println();
#endif // DEBUG_MODE
                }

                if (Xbox.getButtonPress(A))
                {
                        button_bytes[0] |= _BV(B_A);
#ifdef DEBUG_MODE
                        Serial.print("A");
                        Serial.println();
#endif // DEBUG_MODE
                }

                if (Xbox.getButtonPress(B))
                {
                        button_bytes[0] |= _BV(B_B);
#ifdef DEBUG_MODE
                        Serial.print("B");
                        Serial.println();
#endif // DEBUG_MODE
                }

                if (Xbox.getButtonPress(UP))
                {
                        button_bytes[0] |= _BV(B_UP);
#ifdef DEBUG_MODE
                        Serial.print("UP");
                        Serial.println();
#endif // DEBUG_MODE
                }

                if (Xbox.getButtonPress(DOWN))
                {
                        button_bytes[0] |= _BV(B_DOWN);
#ifdef DEBUG_MODE
                        Serial.print("DOWN");
                        Serial.println();
#endif // DEBUG_MODE
                }

                if (Xbox.getButtonPress(LEFT))
                {
                        button_bytes[1] |= _BV(B_LEFT);
#ifdef DEBUG_MODE
                        Serial.print("LEFT");
                        Serial.println();
#endif // DEBUG_MODE
                }

                if (Xbox.getButtonPress(RIGHT))
                {
                        button_bytes[1] |= _BV(B_RIGHT);
#ifdef DEBUG_MODE
                        Serial.print("RIGHT");
                        Serial.println();
#endif // DEBUG_MODE
                }

                if (Xbox.getButtonPress(START))
                {
                        button_bytes[1] |= _BV(B_START);
#ifdef DEBUG_MODE
                        Serial.print("START");
                        Serial.println();
#endif // DEBUG_MODE
                }

                if (Xbox.getButtonPress(BACK))
                {
                        button_bytes[1] |= _BV(B_BACK);
#ifdef DEBUG_MODE
                        Serial.print("BACK");
                        Serial.println();
#endif // DEBUG_MODE
                }

                if (Xbox.getButtonPress(XBOX))
                {
                        button_bytes[1] |= _BV(B_XBOX);
#ifdef DEBUG_MODE
                        Serial.print("XBOX");
                        Serial.println();
#endif // DEBUG_MODE
                }

                if (Xbox.getButtonPress(L3))
                {
                        button_bytes[1] |= _BV(B_L3);
#ifdef DEBUG_MODE
                        Serial.print("L3");
                        Serial.println();
#endif // DEBUG_MODE
                }

                if (Xbox.getButtonPress(R3))
                {
                        button_bytes[1] |= _BV(B_R3);
#ifdef DEBUG_MODE
                        Serial.print("R3");
                        Serial.println();
#endif // DEBUG_MODE
                }

                // if (Xbox.getAnalogHat(LeftHatX) > 200.0 || Xbox.getAnalogHat(LeftHatX) < -200.0)
                {
                        int8_t left_hatx = (int8_t)((float)Xbox.getAnalogHat(LeftHatX) / 256.0);
                        // Lower byte in index 0
                        l_hatx = (uint8_t)(left_hatx);
                        // l_hatx[1] = (uint8_t)(left_hatx >> 8);
#ifdef DEBUG_MODE
                        Serial.print("LeftHatX: ");
                        Serial.println(left_hatx);
#endif // DEBUG_MODE
                }

                if (Xbox.getAnalogHat(LeftHatY) > 200.0 || Xbox.getAnalogHat(LeftHatY) < -200.0)
                {
                        int8_t left_haty = (int8_t)((float)Xbox.getAnalogHat(LeftHatY) / 256.0);
                        // Lower byte in index 0
                        l_haty = (uint8_t)(left_haty);
                        // l_haty[1] = (uint8_t)(left_haty >> 8);
#ifdef DEBUG_MODE
                        Serial.print("LeftHatY: ");
                        Serial.println(left_haty);
#endif // DEBUG_MODE
                }

                if (Xbox.getAnalogHat(RightHatX) > 200.0 || Xbox.getAnalogHat(RightHatX) < -200.0)
                {
                        int8_t right_hatx = (int8_t)((float)Xbox.getAnalogHat(RightHatX) / 256.0);
                        // Lower byte in index 0
                        r_hatx = (uint8_t)(right_hatx);
                        // r_hatx[1] = (uint8_t)(right_hatx >> 8);
#ifdef DEBUG_MODE
                        Serial.print("RightHatX: ");
                        Serial.println(right_hatx);
#endif // DEBUG_MODE
                }

                if (Xbox.getAnalogHat(RightHatY) > 200.0 || Xbox.getAnalogHat(RightHatY) < -200.0)
                {
                        int8_t right_haty = (int8_t)((float)Xbox.getAnalogHat(RightHatY) / 256.0);
                        // Lower byte in index 0
                        r_haty = (uint8_t)(right_haty);
                        // r_haty[1] = (uint8_t)(right_haty >> 8);
#ifdef DEBUG_MODE
                        Serial.print("RightHatY: ");
                        Serial.println(right_haty);
#endif // DEBUG_MODE
                }
        }

        // Filling the array of 13 bytes
        fill_array(arr, (uint8_t)0);

        arr[P_START] = START_BYTE;
        arr[P_BUTTON1] = button_bytes[0];
        arr[P_BUTTON2] = button_bytes[1];
        arr[P_LT] = left_trigger;
        arr[P_RT] = right_trigger;
        // arr[P_LHXH] = l_hatx[1];
        arr[P_LHXL] = l_hatx;
        // arr[P_LHYH] = l_haty[1];
        arr[P_LHYL] = l_haty;
        // arr[P_RHXH] = r_hatx[1];
        arr[P_RHXL] = r_hatx;
        // arr[P_RHYH] = r_haty[1];
        arr[P_RHYL] = r_haty;
}
