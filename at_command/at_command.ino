
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {

Serial.begin(9600);

pinMode(8,OUTPUT); 


Serial.println("Enter AT commands:");

mySerial.begin(38400);
 digitalWrite(8,HIGH); 
}
//MASTER ADDR 0021:09:0017FD
//SLAVE ADDR 0021:09:00187A

void loop()

{
 

if (mySerial.available())

Serial.write(mySerial.read());

if (Serial.available())

mySerial.write(Serial.read());

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
