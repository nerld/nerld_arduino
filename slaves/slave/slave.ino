// Wire Slave Receiver
#include "NerldProtocol.h"
#include <Wire.h>

NerldProtocol nerldProtocol = NerldProtocol();

int DEFAULT_ADDR = 99;
int ADDR = NULL;

void setup()
{
  Serial.begin(9600); 
  Serial.println("INFO::setup: Initialising Basic Slave.");

  
  Wire.begin(DEFAULT_ADDR);
  
  
  while (ADDR == NULL) {
    int newAddress = nerldProtocol.requestAddresssFromMaster();
    ADDR = newAddress;
    delay(1000);    
  }
  
  Serial.print("INFO::setup: Setting the slave address to ");
  Serial.println(ADDR);
  TWAR = ADDR << 1;
  
  Wire.onRequest(healthCheck);
  Wire.onReceive(receiveCommand);
  
  Serial.println("INFO::setup: Registering the address with the master.");
  nerldProtocol.sendCommandToMaster(ADDR, 0, 0);
  Serial.println("INFO::setup: Successfully registered.");
  Serial.println("INFO::setup: Basic Slave initialised.");
}

void loop()
{
  delay(1000);
}

void healthCheck() {
  Serial.println("INFO::healthCheck: Called.");
  Wire.write("1");
  Serial.println("INFO::healthCheck: Done.");
}

void receiveCommand(int numOfBytes) {
  Serial.println("INFO::receiveCommand: Command received");
  
  int i = 0;
  while(Wire.available() && i < numOfBytes)
  {
    char c = Wire.read();
    Serial.println(c);
    i++;
  }
}

void executeCommand(int address, int command, String value) {
  switch (command) {
    case 0:
      Serial.println("INFO::executeCommand: setPinMode called.");
      break;
    case 1:
      Serial.println("INFO::executeCommand: digitalWritePin called.");
      break;
    default:
      Serial.println("INFO::executeCommand: invalid command called.");
      break; 
  }
}

int setPinMode(int pin, String mode) {
  if (validateDigitalPin(pin)) {
    if (mode == "INPUT") {
      pinMode(pin, INPUT);
      return INPUT;
    } else if (mode == "OUTPUT") {
      pinMode(pin, OUTPUT);
      return OUTPUT;
    }  
  }
}

int digitalWritePin(int pin, String value) {
  if (validateDigitalPin(pin)) {
    if (value == "HIGH") {
      digitalWrite(pin, HIGH);
      return HIGH;
    } else if (value == "LOW") {
      digitalWrite(pin, LOW);
      return LOW;
    }
  }
}

bool validateDigitalPin(int pin) {
  return (pin >= 0 && pin <= 13);
}


int readDigitalPin(int pin) {
  return digitalRead(pin);
}

// analog read
float readAnalogPin(int pin) {
  return analogRead(pin);
}

// analog write
float writeAnalogPin(int pin, float value) {
  analogWrite(pin, value);
  return value;
}
