#include <Wire.h>
#include "NerldProtocol.h"
#include "BaseSlave.h"

#define MAX_SLAVES 15

NerldProtocol nerldProtocol = NerldProtocol();

HardwareSerial *port;

BaseSlave slaves[MAX_SLAVES];

int NEXT_FREE_ADDR = 2;
int slaveCount = 0;

void setup()
{ 
  // Bridge Serial Port
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

  int nextAddress =nextAvaiableAddress();

  if (nextAddress >= MAX_SLAVES) {
    Serial.print("INFO::nextAvaiableAddressRequest: No slave address available.");
    Wire.write(99);
  } else {
    Wire.write(nextAddress);
    delay(100);
    Serial.print("INFO::nextAvaiableAddressRequest: Returned ");
    Serial.println(nextAddress);
  }
}

void receiveCommand(int numOfBytes)
{ 
  Serial.println("INFO::receiveCommand: Command received");
  String message[3]; 
  nerldProtocol.receiveCommand(numOfBytes, message);

  int address = message[0].toInt();
  int command = message[1].toInt();
  String value = message[2];

  executeCommand(address, command, value);
}

void executeCommand(int address, int command, String value) {
  Serial.print("INFO::executeCommand: From slave with address ");
  Serial.print(address);
  Serial.print(" command ");
  Serial.print(command);
  Serial.print(" and value ");
  Serial.println(value);

  switch (command) {
    case 0:
      registerSlave(address);
      break;
    case 1:
      removeSlave(address);
    default:
      break;
  }
}

void registerSlave(int address) {
  Serial.println("INFO:registerSlave: Registering Slave");
  slaves[address] = BaseSlave(address);
  Serial.println("INFO:registerSlave: Slave Registered.");
}

void removeSlave(int address) {
  Serial.println("INFO:removeSlave: Removing Slave");
  slaves[address] = NULL;
}

int nextAvaiableAddress() {
  for (int i = 0; i < MAX_SLAVES; i++) {
    if (slaves[i] == NULL) {
      return i
    }
  }
  return 99;
}