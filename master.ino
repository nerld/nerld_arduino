#include <Wire.h>
#include "SwitchSlave.h"

HardwareSerial *port;

int NEXT_FREE_ADDR = 2;
SwitchSlave switchSlaves[99];
int slaveCount = 0;

// COMMANDS
const int REGISTER_SLAVE = 0;
const int CHECK_FOR_COMMANDS = 1;

void setup()
{ 
  
  // Initialise the serial ports
  port = &Serial1;
  port->begin(9600);

  Serial.begin(4800); // debug serial

  // Initialise i2c mechanism
  Wire.begin(1);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
}

void loop()
{  
  // if (slaveCount != 0) {
  //   switchSlaves[slaveCount-1].toggle();
  //   writeToServer(switchSlaves[slaveCount-1].m_address, switchSlaves[slaveCount-1].m_currentState);
  // }
  
  // if (SERVO_ADDR != NULL) {
  //   int r = rand() % 180;
  //    writeServo(r); 
  // }
  
  requestCommandsFromServer();
  delay(1000);
}

void requestCommandsFromServer() {
  sendCommandToServer(CHECK_FOR_COMMANDS, NULL, NULL);
  // receive commands from server
  while (port->available()){
    char buffer[6];
    int i = 0;
    while (port->available() && i < 6) {
      char inByte = port->read();
      buffer[i] = inByte;
      i++;
    }

    char addrStr[3];
    addrStr[0] = buffer[0];
    addrStr[1] = buffer[1];
    addrStr[2] = '\0';

    char valStr[3];
    valStr[0] = buffer[2];
    valStr[1] = buffer[3];
    valStr[2] = '\0';

    char typeStr[3];
    typeStr[0] = buffer[4];
    typeStr[1] = buffer[5];
    typeStr[2] = '\0';

    int addr = atoi(addrStr);
    int val = atoi(valStr);
    int type = atoi(typeStr);
    executeSlaveCommand(type, addr, val);
    delay(100);
  }
}

void executeSlaveCommand(int type, int addr, int val) {
  switch (type) {
    case 0:
      switchSlaves[addr].execute(val);
      break;
    default:
      break;
  }
  // notify server that command is completed
}

// int writeServo(int angle) {
//   Wire.beginTransmission(SERVO_ADDR);
//   Wire.write(angle);
//   Wire.endTransmission();
// }

void requestEvent() { 
 Wire.write(NEXT_FREE_ADDR);
 NEXT_FREE_ADDR++; 
}

void receiveEvent(int howMany)
{ 
  char buffer[6];
  int i = 0;
  while(Wire.available() && i < 6)
  {
    char c = Wire.read();
    buffer[i] = c;
    i++;
  }

  char commandStr[3];
  char addrStr[3];
  char typeStr[2];

  commandStr[0] = buffer[0];
  commandStr[1] = buffer[1];
  commandStr[2] = '\0';
  
  addrStr[0] = buffer[2];
  addrStr[1] = buffer[3];
  addrStr[2] = '\0';

  typeStr[0] = buffer[4];
  typeStr[1] = '\0';

  int command = atoi(commandStr);
  int addr = atoi(addrStr);
  int type = atoi(typeStr);

  doReceiveEventCommand(command, addr, type);
}

void doReceiveEventCommand(int command, int addr, int type) {
  switch (command) {
    case 0:
      // Register Slave
      if (type == 0) {
        switchSlaves[addr] = SwitchSlave(addr);
        slaveCount++;
        // Register the slave with the server
        sendCommandToServer(REGISTER_SLAVE, addr, type);
      }
      break;
    case 1:
      // do something
      break;
    default:
      // invalid command
      break;
  }
}

void sendCommandToServer(int command, int address, int value) {
  port->print(command);
  port->print('\t');
  port->print(address);
  port->print('\t');
  port->print(value);
  port->println();
}