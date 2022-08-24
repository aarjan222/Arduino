#include <TFMPlus.h>
#include <SoftwareSerial.h>

int distance = 0, strength = 0;
TFMPlus tfmini;
//SoftwareSerial Serial3;          //The only value that matters here is the first one, 2, Rx

void setup()
{
  Serial.begin(115200);       //Initialize hardware serial port (serial debug port)
  while (!Serial);            // wait for serial port to connect. Needed for native USB port only
  Serial.println ("Initializing...");
  Serial3.begin(115200);    //Initialize the data rate for the SoftwareSerial port
  tfmini.begin(&Serial3);            //Initialize the TF Mini sensor
  delay(1000);
}
  
void loop()
{
 getdata(&distance, &strength);
    if (distance)
    {
      Serial.print(distance);
      Serial.print("cm\t");
      Serial.print("strength: ");
      Serial.println(strength);
    }
  delay(100);
}

void getdata(int* distance, int* strength)
{
  static char i = 0;
  char j = 0;
  int checksum = 0;
  static int rx[9];
  if (Serial3.available())
  {
//    Serial.println("Working in ");
    rx[i] = Serial3.read();
    if (rx[0] != 0x59)
    {
      i = 0;
    }
    else if (i == 1 && rx[1] != 0x59)
    {
      i = 0;
    }
    else if (i == 8)
    {
      for (j = 0; j < 8; j++)
      {
        checksum += rx[j];
      }
      if (rx[8] == (checksum % 256))
      {
        *distance = rx[2] + rx[3] * 256;
        *strength = rx[4] + rx[5] * 256;
      }
      i = 0;
    }
    else
    {
      i++;
    }
  }
}
