#include <Wire.h>
#include "SwitchSlave.h"

int NEXT_FREE_ADDR = 2;
int SWITCH_ADDR = NULL;
int SERVO_ADDR = NULL;
SwitchSlave switchSlaves[99];
int switchCount = 0;

void setup()
{
  Serial.begin(9600);
  Wire.begin(1); // join i2c bus (address optional for master)
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
}

byte x = 0;

void loop()
{
  
  Serial.print("Switch Slaves count: ");
  Serial.println(switchCount);

  if (switchCount != 0) {
    switchSlaves[switchCount-1].toggle();
  }
  
  if (SERVO_ADDR != NULL) {
    int r = rand() % 180;
     writeServo(r); 
  }
  delay(500);
}

int toggleSwitchSlave(SwitchSlave ss) {
  Wire.requestFrom(ss.m_address,1);
  while(Wire.available()) {
    Serial.println(Wire.read());
  }
}

int writeServo(int angle) {
  Wire.beginTransmission(SERVO_ADDR);
  Wire.write(angle);
  Wire.endTransmission();
}

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

  doCommand(command, addr, type);

}

void doCommand(int command, int addr, int type) {
  switch (command) {
    case 0:
      // Register Slave
      if (type == 0) {
        switchSlaves[switchCount] = SwitchSlave(addr);
        switchCount++;
      }
      break;
    case 1:
      // do something
      break;
    default:
      // do something
      Serial.println("INVALID COMMAND");
  }
}