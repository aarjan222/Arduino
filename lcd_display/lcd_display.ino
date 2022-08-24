#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0X27, 16, 2);

void setup() {
  // put your setup code here, to run once:
lcd.init();
lcd.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:
lcd.setCursor(0, 0);
lcd.leftToRight();
lcd.print("Robotics is Fun");
lcd.cursor_on();
lcd.setCursor(0, 1);
lcd.print("It is Hangover too!!");
lcd.autoscroll();
delay(1000);
lcd.setCursor(0, 2);
lcd.print("Coding is Fun");
lcd.blink_on();
//lcd.noBacklight();
}
