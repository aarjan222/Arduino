#include <Wire.h>
#include "SPI.h"
#include "TFT_22_ILI9225.h"

#define TFT_RST A4
#define TFT_RS A3
#define TFT_CS A5
#define TFT_SDI A2
#define TFT_CLK A1
#define TFT_LED 0
#define ARDUINO_PRO_MICRO_I2C_RECEIVE_ADDRESS (4)

TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_SDI, TFT_CLK, TFT_LED);

float num = 123.456786;

struct FloatData {
  float float1;
  float float2;
};

FloatData receivedData;

void setup() {
  Wire.begin(ARDUINO_PRO_MICRO_I2C_RECEIVE_ADDRESS);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);

  tft.begin();
  tft.fillRectangle(0, 0, tft.maxX() - 1, tft.maxY() - 1, COLOR_WHITE);
  tft.setBackgroundColor(COLOR_WHITE);
  tft.setOrientation(3);
}


void loop() {
  Serial.println("data to arduino from stm");

  tft.setFont(Terminal11x16);
  tft.drawText(0, 50, "ROLLER1 = " + String(receivedData.float1), COLOR_BLACK);

  tft.setFont(Terminal11x16);
  tft.drawText(0, 80, "ROLLER2 = " + String(receivedData.float2), COLOR_BLACK);
}

void receiveEvent(int numBytes) {
  if (numBytes >= sizeof(FloatData)) {

    Wire.readBytes(reinterpret_cast<byte*>(&receivedData), sizeof(receivedData));


    Serial.print("Received Float 1: ");
    Serial.print(receivedData.float1);
    Serial.print("\tReceived Float 2: ");
    Serial.println(receivedData.float2);
  }
}
