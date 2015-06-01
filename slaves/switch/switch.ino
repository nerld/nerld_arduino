// Wire Slave Receiver
#include "NerldProtocol.h"
#include <Wire.h>

NerldProtocol nerldProtocol = NerldProtocol();

int DEFAULT_ADDR = 99;
int ADDR = NULL;

int pinState[14];

void setup()
{
  Serial.begin(9600); 
  Serial.println("INFO::setup: Initialising Switch Slave.");


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
  nerldProtocol.sendCommandToMaster(ADDR, 0, 0);
  Serial.println("INFO::setup: Successfully registered.");


  // init pins and set to LOW
  for(int i = 0; i < 14; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
    pinState[i] = 0;
  }

  Serial.println("INFO::setup: Switch Slave initialised.");
}

void loop()
{

  String str = "";

  for(int i = 0; i < 14; i++) {
    str += pinState[i];
    if (i != 13) {
      str += ',';
    }
  }

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

  if (command == 0) { // toggleOff
    digitalWrite(pin, LOW);
    pinState[pin] = 0;    
  } 
  else if (command == 1) { // toggleOn
    digitalWrite(pin, HIGH);
    pinState[pin] = 1;    
  } 
  else {
    Serial.println("invalid command"); 
  }
}




