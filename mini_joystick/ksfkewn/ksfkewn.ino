int ledpin=11;
int button=2;
int vrx=A0;
int vry=A1;
int vrx_val, vry_val, led_val_x, led_val_y, button_val;
void setup() {
  
Serial.begin(9600);
pinMode(ledpin,OUTPUT);
pinMode(button,INPUT);
//attachInterrupt(digitalpintointerrupt)

}

void loop() {
  // put your main code here, to run repeatedly:
vrx_val=analogRead(vrx);
vry_val=analogRead(vry);
button_val=digitalRead(button);

led_val_x=map(vrx_val,0,1023,0,255);

Serial.print("vrx_val");
Serial.print("\t");
Serial.print("\n");
Serial.print("vry_val");
Serial.print("\t");

analogWrite(ledpin, led_val_x);

}
