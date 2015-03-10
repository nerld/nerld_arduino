#include <Wire.h>
#include "SwitchSlave.h"
#include "ServoSlave.h"
#include "TMP36Slave.h"

HardwareSerial *port;

int NEXT_FREE_ADDR = 2;
SwitchSlave switchSlaves[20];
TMP36Slave tmp36Slaves[20];
int slaveCount = 0;

// COMMANDS
const int REGISTER_SLAVE = 0;
const int CHECK_FOR_COMMANDS = 1;
const int SEND_VALUE = 2;

void setup()
{ 
  
  // Initialise the serial ports
  port = &Serial1;
  port->begin(9600);

  Serial.begin(4800); // debug serial

  Serial.println('Serial ports are up!');

  // Initialise i2c mechanism
  Wire.begin(1);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
}

void loop()
{    
  requestCommandsFromServer();
  delay(1000);
}

void requestCommandsFromServer() {
  sendCommandToServer(CHECK_FOR_COMMANDS, NULL, NULL);
  // receive commands from server
  while (port->available()){
    char buffer[9];
    int i = 0;
    while (port->available() && i < 9) {
      char inByte = port->read();
      buffer[i] = inByte;
      i++;
    }

    char typeStr[3];
    typeStr[0] = buffer[0];
    typeStr[1] = buffer[1];
    typeStr[2] = '\0';
    
    char addrStr[3];
    addrStr[0] = buffer[2];
    addrStr[1] = buffer[3];
    addrStr[2] = '\0';
    
    char valStr[6];
    valStr[0] = buffer[4];
    valStr[1] = buffer[5];
    valStr[2] = buffer[6];
    valStr[3] = buffer[7];
    valStr[4] = buffer[8];
    valStr[5] = '\0';

    int addr = atoi(addrStr);
    int val = atoi(valStr);
    int type = atoi(typeStr);
    
    Serial.println(addr);
    Serial.println(type);
    Serial.println(val);
    
    executeSlaveCommand(type, addr, val);
    delay(100);
  }
}

void executeSlaveCommand(int type, int addr, int val) {
  switch (type) {
    case 0:
      switchSlaves[addr].execute(val);
      break;
    case 3:
      tmp36Slaves[addr].execute(val);
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
  char buffer[11];
  int i = 0;
  while(Wire.available() && i < 11)
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

  doReceiveEventCommand(command, addr, type, buffer);
}

void doReceiveEventCommand(int command, int addr, int type, char* buffer) {
  Serial.println("doReceiveEventCommand called.");
  switch (command) {
    case 0:
      // Register Slave
      Serial.println("doReceiveEventCommand::RegisterSlave called.");
      if (type == 0) {
        switchSlaves[addr] = SwitchSlave(addr);
        slaveCount++;
        // Register the slave with the server
        sendCommandToServer(REGISTER_SLAVE, addr, type);
      } else if (type == 3) {
        tmp36Slaves[addr] = TMP36Slave(addr);
        slaveCount++;
        // Register the slave with the server
        sendCommandToServer(REGISTER_SLAVE, addr, type);
      }
      break;
    case 1:
      // Send float to server
      char tempFloatChar[5];
      tempFloatChar[0] = buffer[4];
      tempFloatChar[1] = buffer[5];
      tempFloatChar[2] = buffer[6];
      tempFloatChar[3] = buffer[7];
      tempFloatChar[4] = '\0';
      sendCommandToServer(SEND_VALUE, addr, atoi(tempFloatChar));
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
