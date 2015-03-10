// Wire Slave Receiver
#include "NerldProtocol.h"
#include <Wire.h>

NerldProtocol nerldProtocol = NerldProtocol();

int DEFAULT_ADDR = 99;
int ADDR = NULL;
int status = 0;
int LED = 10;

void setup()
{
  Serial.println("INFO::setup: Initialising Switch Slave.");
  Serial.begin(9600); 
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);

  Wire.begin(DEFAULT_ADDR);
  Wire.onRequest(toggleSwitchRequest);
  
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
  Serial.println("INFO::setup: Switch Slave initialised.");
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
