#include <SIM800L.h>
SIM800L gsm(10, 11);

void handleSMS(String number, String message) {
  Serial.println("number: " + number + "\nMessage: " + message);
}

void handleCall(String number) {
  Serial.println("New call from " + number);
}


void setup() {
  Serial.begin(9600);
  gsm.begin(9600);

  gsm.setSMSCallback(handleSMS);
  gsm.setCallCallback(handleCall);
}

void loop() {
  gsm.listen();
}
