#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  lcd.begin(16, 2);
}

void loop() {
  lcd.setCursor(5,0);//rows, cols
  lcd.print("hello sexxy");
  lcd.setCursor(2, 1);
  lcd.blink();
  delay(300);
  lcd.print("ok");
}
