#include <SoftwareSerial.h>
char c = ' ';
SoftwareSerial mySerial(10, 11);  // RX, TX

void setup() {

  Serial.begin(9600);

  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);

  Serial.println("Enter AT commands:");

  mySerial.begin(115200);
}
// SLAVE +ADDR:0019:10:097B49
//MASTER +ADDR:0019:10:09564F
void loop()

{

  if (mySerial.available()) {
    c = mySerial.read();
    Serial.print(c);
    //Serial.write(mySerial.read());
  }
  if (Serial.available())

    mySerial.write(Serial.read());
}