// Wire Slave Receiver
#include "NerldProtocol.h"
#include <Wire.h>

NerldProtocol nerldProtocol = NerldProtocol();

int DEFAULT_ADDR = 99;
int ADDR = NULL;

int motorHigh = 2;
int motorLow = 3;
boolean isOn = false;

void setup()
{
  Serial.begin(9600); 
  Serial.println("INFO::setup: Initialising Motor Slave.");


  Wire.begin(DEFAULT_ADDR);


  while (ADDR == NULL) {
    int newAddress = nerldProtocol.requestAddresssFromMaster();
    ADDR = newAddress;
    delay(1000);    
  }

  Serial.print("INFO::setup: Setting the slave address to ");
  Serial.println(ADDR);
  TWAR = ADDR << 1;

  //  Wire.onRequest(healthCheck);
  Wire.onReceive(receiveCommand);

  Serial.println("INFO::setup: Registering the address with the master.");
  nerldProtocol.sendCommandToMaster(ADDR, 0, 3);
  Serial.println("INFO::setup: Successfully registered.");

  pinMode(motorHigh, OUTPUT);
  pinMode(motorLow, OUTPUT);
  digitalWrite(motorHigh, LOW);
  digitalWrite(motorLow, LOW);  

  Serial.println("INFO::setup: Motor Slave initialised.");
}

void loop()
{

  String str = String(isOn);

  Serial.println(str);

  nerldProtocol.sendCommandToMaster(ADDR, 1, str);

  delay(1000);
}

void receiveCommand(int numOfBytes) {
  Serial.println("INFO::receiveCommand: Command received");

  String message[3];

  for (int j = 0; j < 3; j++) {
    message[j] = "";
  }

  int i = 0;
  int currentArgIdx = 0;
  while(Wire.available() && i < numOfBytes)
  {
    char c = Wire.read();

    if (c != ':') {
      message[currentArgIdx] += c;
    } 
    else {
      currentArgIdx++;
    }
    i++;
  }

  int address = (int) message[0].toInt();
  int command = (int) message[1].toInt();
  int pin = (int) message[2].toInt();

  if (command == 0) { // turn off motor
    off();
  } 
  else if (command == 1) { // turn on motor forward
    onForward();
  }
  else if (command == 2) { // turn on motor reverse
    onReverse();
  } 
  else {
    Serial.println("invalid command"); 
  }
}

void onForward() {
  digitalWrite(motorHigh, HIGH);
  digitalWrite(motorLow, LOW);
  isOn = true;
}

void onReverse() {
  digitalWrite(motorHigh, LOW);
  digitalWrite(motorLow, HIGH);
  isOn = true;
}

void off() {
  digitalWrite(motorHigh, LOW);
  digitalWrite(motorLow, LOW);  
  isOn = false;
} 



