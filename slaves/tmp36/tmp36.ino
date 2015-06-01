// Wire Slave Receiver
#include "NerldProtocol.h"
#include <Wire.h>

NerldProtocol nerldProtocol = NerldProtocol();

int DEFAULT_ADDR = 99;
int ADDR = NULL;

float readings[4];

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
  nerldProtocol.sendCommandToMaster(ADDR, 0, 2);
  Serial.println("INFO::setup: Successfully registered.");

  // init pins and set to LOW
  for(int i = 0; i < 4; i++) {
    readings[i] = 0.0;
  }

  Serial.println("INFO::setup: Analog Read Slave initialised.");
}

void loop()
{

  String str = "";

  for(int i = 0; i < 4; i++) {

    int analogReading = analogRead(i);  

    // converting that reading to voltage, for 3.3v arduino use 3.3
    float voltage = analogReading * 5.0;
    voltage /= 1024.0; 

    readings[i] = (voltage - 0.5) * 100 ;

    char charBuf[10];

    dtostrf(readings[i], 5, 4, charBuf);

    str += charBuf;
    if (i != 3) {
      str += ',';
    }
  }

  Serial.println(str);

  nerldProtocol.sendCommandToMaster(ADDR, 1, str);

  delay(1000);
}






