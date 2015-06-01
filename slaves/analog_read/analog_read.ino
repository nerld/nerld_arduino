// Wire Slave Receiver
#include "NerldProtocol.h"
#include <Wire.h>

NerldProtocol nerldProtocol = NerldProtocol();

int DEFAULT_ADDR = 99;
int ADDR = NULL;

int pinState[4];

void setup()
{
  Serial.begin(9600); 
  Serial.println("INFO::setup: Initialising Analog Read Slave.");


  Wire.begin(DEFAULT_ADDR);


  while (ADDR == NULL) {
    int newAddress = nerldProtocol.requestAddresssFromMaster();
    ADDR = newAddress;
    delay(1000);    
  }

  Serial.print("INFO::setup: Setting the slave address to ");
  Serial.println(ADDR);
  TWAR = ADDR << 1;

  Serial.println("INFO::setup: Registering the address with the master.");
  nerldProtocol.sendCommandToMaster(ADDR, 0, 0);
  Serial.println("INFO::setup: Successfully registered.");

  // init pins and set to LOW
  for(int i = 0; i < 4; i++) {
    pinState[i] = 0;
  }

  Serial.println("INFO::setup: Analog Read Slave initialised.");
}

void loop()
{

  String str = "";

  for(int i = 0; i < 4; i++) {
    pinState[i] = analogRead(i);
    str += pinState[i];
    if (i != 3) {
      str += ',';
    }
  }

  Serial.println(str);

  nerldProtocol.sendCommandToMaster(ADDR, 1, str);

  delay(1000);
}




