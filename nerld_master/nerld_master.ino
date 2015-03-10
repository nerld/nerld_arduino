#include <Wire.h>
#include "NerldProtocol.h"

NerldProtocol nerldProtocol = NerldProtocol();

HardwareSerial *port;

int NEXT_FREE_ADDR = 2;
int slaveCount = 0;

void setup()
{ 
  // Bridge Serail
  port = &Serial1;
  port->begin(9600);

  Serial.begin(4800); // debug serial

  Serial.println("INFO::setup: Serial ports are up!");

  Serial.println("INFO::setup: Initialising I2C Mechanism...");
  Wire.begin(1);
  Wire.onRequest(nextAvaiableAddressRequest);
  Wire.onReceive(receiveCommand);
  Serial.println("INFO::setup: I2C successfully started.");
}

void loop()
{    
  delay(1000);
}

void nextAvaiableAddressRequest() { 
  Serial.println("INFO::nextAvaiableAddressRequest: Next slave address requested.");
  Wire.write(NEXT_FREE_ADDR);
  delay(100);
  Serial.print("INFO::nextAvaiableAddressRequest: Returned ");
  Serial.println(NEXT_FREE_ADDR);
  NEXT_FREE_ADDR++; 
}

void receiveCommand(int numOfBytes)
{ 
  Serial.println("INFO::receiveCommand: Command received");
  String message[3]; 
  nerldProtocol.receiveCommand(numOfBytes, message);
  Serial.print("INFO::receiveCommand: From slave with address ");
  Serial.print(message[0]);
  Serial.print(" command ");
  Serial.print(message[1]);
  Serial.print(" and value ");
  Serial.println(message[2]);
}
