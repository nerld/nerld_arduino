#include <Wire.h>
#include "NerldProtocol.h"
#include "BaseSlave.h"

#define MAX_SLAVES 15

NerldProtocol nerldProtocol = NerldProtocol();

HardwareSerial *port;

BaseSlave slaves[MAX_SLAVES];

int ADDR = 1;
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
  Wire.begin(ADDR);
  Wire.onRequest(nextAvaiableAddressRequest);
  Wire.onReceive(receiveCommand);
  Serial.println("INFO::setup: I2C successfully started.");
}

void loop()
{   
  sendCommandToServer("testing");
  delay(10000);
}

void healthChecker() {
  Serial.println("INFO::healthChecker: Starting slave check...");
  // do a health check on all slaves
  for (int i = 0; i < MAX_SLAVES; i++) {
    if (slaves[i].m_address != 0) {
      Serial.println(slaves[i].m_address);
      int response = Wire.requestFrom(slaves[i].m_address, 1);
      if (response == 0) {
       Serial.print("INFO::healthChecker: Slave at address ");
       Serial.print(slaves[i].m_address);
       Serial.println(" did not respond to health check. Removing...");
       removeSlave(i);
       delay(100);
      }
    }
  }
  Serial.println("INFO::healthChecker: Slave check complete.");
}

void nextAvaiableAddressRequest() { 
  Serial.println("INFO::nextAvaiableAddressRequest: Next slave address requested.");

  int nextAddress = nextAvaiableAddress();

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
      registerSlave(address, value.toInt());
      break;
    case 1:
      removeSlave(address);
    default:
      break;
  }
}

void registerSlave(int address, int type) {
  Serial.println("INFO:registerSlave: Registering Slave");
  slaves[address-2].m_address = address;
  slaves[address-2].m_type = type;
  slaveCount++;
  Serial.println("INFO:registerSlave: Slave Registered.");
}

void removeSlave(int index) {
  Serial.println("INFO:removeSlave: Removing Slave");
  slaves[index].m_address = 0;
  slaveCount--;
}

int nextAvaiableAddress() {
  for (int i = 0; i < MAX_SLAVES; i++) {
    if (slaves[i].m_address == 0) {
      return i+2;
    }
  }
  return 99;
}

void sendCommandToServer(String command) {
  port->println(command);
}
