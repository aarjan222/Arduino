#include <Servo.h>

// Rotary Encoder Inputs
 #define inputCLK 4
 #define inputDT 5
 
 // LED Outputs
//  #define ledCW 8
//  #define ledCCW 9

Servo myservo;
 
 int counter = 0; 
 int currentStateCLK;
 int previousStateCLK; 
 
 String encdir ="";
 
 void setup() { 
   
   // Set encoder pins as inputs  
   pinMode (inputCLK,INPUT);
   pinMode (inputDT,INPUT);
   
   // Set LED pins as outputs
  //  pinMode (ledCW,OUTPUT);
  //  pinMode (ledCCW,OUTPUT);
   
   // Setup Serial Monitor
   Serial.begin (9600);
   myservo.attach(9);
   // Read the initial state of inputCLK
   // Assign to previousStateCLK variable
   previousStateCLK = digitalRead(inputCLK);

 } 
 
 void loop() { 
  
  // Read the current state of inputCLK
   currentStateCLK = digitalRead(inputCLK);
    
   // If the previous and the current state of the inputCLK are different then a pulse has occured
   if (currentStateCLK != previousStateCLK){ 
       
     // If the inputDT state is different than the inputCLK state then 
     // the encoder is rotating counterclockwise
     if (digitalRead(inputDT) != currentStateCLK) { 
       counter ++;
      //  encdir ="CW";
      //  digitalWrite(ledCW, 1);
      //  digitalWrite(ledCCW, 0);
      if(counter == 180)
      {
        counter = 180;
      }
       
     } else {
       // Encoder is rotating clockwise
       counter --;
      //  encdir ="CCW";
      //  digitalWrite(ledCW, 0);
      //  digitalWrite(ledCCW, 1);
      if(counter == 0)
      {
        counter = 0;
      }
       
     }

      myservo.write(counter);

    //  Serial.print("Direction: ");
    //  Serial.print(encdir);
     Serial.print("Position: ");
     Serial.println(counter);
   } 
   // Update previousStateCLK with the current state
   previousStateCLK = currentStateCLK; 
 }