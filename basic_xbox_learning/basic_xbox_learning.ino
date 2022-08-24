#define __ps4_joystick_
//#define __xbox_joystick_

#ifdef __xbox_joystick_
#include <XBOXUSB.h>
#elif defined(__ps4_joystick_)
#include <PS4USB.h>
#endif

#include <SPI.h>

USB Usb;

#ifdef __xbox_joystick_
XBOXUSB Xbox(&Usb);
#elif defined(__ps4_joystick_)
PS4USB PS4(&Usb);
#endif



uint8_t l_hatx = 0;

void setup() {
 Serial.begin(115200);
 while(!Serial){
  Serial.println("Serial not available!!");
 }
 if (Usb.Init() == -1){
  Serial.print(F("\r\nOSC did not start"));
  while (1);
 }
 Serial.print(F("\r\n PS4 USB Library Started"));
}

void loop() {
 Usb.Task();
 #ifdef __xbox_joystick_
 if (Xbox.Xbox360Connected){
//  if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500 || Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500 || Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500 || Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
//      if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500) {
//        Serial.print(F("LeftHatX: "));
//        Serial.print(PS4.getAnalogHat(LeftHatX));
//        Serial.print("\t");
//
//        Serial.print(F("LeftHatY: "));
//        Serial.print(PS4.getAnalogHat(LeftHatY));
//        Serial.print("\t");
//        Serial.println("");
//        delay(100);

int8_t left_hatx = Xbox.getAnalogHat(LeftHatX)/256;   
Serial.print("Left HatX in signed 8 bit integer: ");
Serial.print(left_hatx);

Serial.print("         ");
//Serial.print("Left HatX in signed 16 bit integer: ");
//Serial.println(Xbox.getAnalogHat(LeftHatX));

l_hatx = (uint8_t)(left_hatx);
Serial.print("Left HatX in unsigned 8 bit integer");
Serial.print(l_hatx);
Serial.println("");



//      }
}

#elif defined(__ps4_joystick_)
if(PS4.connected()){
  if(PS4.getAnalogHat(LeftHatX) > 135 || PS4.getAnalogHat(LeftHatX) < 117)
  {
      Serial.print("LeftHatX in unsigned 8 bit integer:");
      Serial.print(PS4.getAnalogHat(LeftHatX)); 

      int8_t l_hatx = (int8_t)PS4.getAnalogHat(LeftHatX);
      
      Serial.print("LeftHatX in signed 8 bit integer:");
      Serial.print(l_hatx); 
      Serial.println("");
  }
}
#endif

} 
//}
//}
