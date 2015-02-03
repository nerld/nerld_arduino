// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
#include <Servo.h> 
 
Servo myservo;
int DEFAULT_ADDR = 99;
int ADDR = NULL;
int LED = 10;

void setup()
{
  Serial.begin(9600); 
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 

  Wire.begin(DEFAULT_ADDR);
  Wire.onReceive(receiveEvent);
  Wire.requestFrom(1,1);
  while (ADDR == NULL && Wire.available()) {
    ADDR = (Wire.read());
    Serial.print(ADDR);
   delay(100);
   TWAR = ADDR << 1;
   }
//   
//   byte setupArray[] = {1, 2, 3};
//   
//
//    //send addr + func+type
//   Wire.beginTransmission(1);
//   Wire.write(setupArray, 3);
//  
   
 

  
 
  
}

void loop()
{
  
  delay(100);
}

//void recieveServoWrite() {
//  
//  
//  myservo.write(Wire.read());
//  Wire.write(myservo.read);
//  
//  delay(500);
//
//   
//}

void receiveEvent(int howMany)
{
  while(1 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
  myservo.write(x);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()

