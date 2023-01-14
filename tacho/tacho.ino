#define sensor 2
int delay1() {
  //unsigned int long k;
  int i, j;
  unsigned int count = 0;
    for (j = 0; j < 1000; j++) {
      if (digitalRead(sensor)) {//high and enter = black
        count++;
        while (digitalRead(sensor))// white and exit
          ;
      }
    }
  return count;
}
void setup() {
  Serial.begin(9600);
  pinMode(sensor, INPUT);
  // delay(2000);
}
void loop() {
  unsigned int time = 0, RPM = 0;
  time = delay1();
  RPM = (time * 12) / 3;
  // delay(2000);
  Serial.println(RPM);
  // delay(5000);
}