// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

int DEFAULT_ADDR = 99;
int ADDR = NULL;
int status = 0;
int LED = 10;

void setup()
{
  Serial.begin(9600); 
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);

  Wire.begin(DEFAULT_ADDR);
  Wire.onRequest(toggleSwitchRequest);
  Wire.requestFrom(1,1);
  while (ADDR == NULL && Wire.available()) {
    ADDR = (Wire.read());
    delay(100);
    TWAR = ADDR << 1;
  }

   //send addr + func+type
  String command = "00";
  String type = "0";
  String address = "0";
  if (ADDR < 10) {
    address = address + ADDR;
  } else {
    address = "";
    address = address + ADDR;
  }
  String commandString = command + address;
  commandString = commandString  + type; 
  Serial.println(commandString);
  char buffer[6];
  commandString.toCharArray(buffer, 6);
  Serial.println(buffer);
  Wire.beginTransmission(1);
  Wire.write(buffer);
  Wire.endTransmission();
}

void loop()
{
  
  delay(100);
}

void toggleSwitchRequest() {
  
  if(status == 0)
  digitalWrite(LED, HIGH);
  else
  digitalWrite(LED, LOW);
  
  status = !status;
  
  Wire.write(status);

   
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()

