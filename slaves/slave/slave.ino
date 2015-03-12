// Wire Slave Receiver
#include "NerldProtocol.h"
#include <Wire.h>

NerldProtocol nerldProtocol = NerldProtocol();

int DEFAULT_ADDR = 99;
int ADDR = NULL;

void setup()
{
  Serial.println("INFO::setup: Initialising Basic Slave.");
  Serial.begin(9600); 
  
  // pinMode(10, OUTPUT);
  // digitalWrite(10, LOW);

  Wire.begin(DEFAULT_ADDR);
  // Wire.onRequest(toggleSwitchRequest);
  
  while (ADDR == NULL) {
    int newAddress = nerldProtocol.requestAddresssFromMaster();
    ADDR = newAddress;
    delay(1000);    
  }
  
  Serial.print("INFO::setup: Setting the slave address to ");
  Serial.println(ADDR);
  TWAR = ADDR << 1;
  
  Serial.println("INFO::setup: Registering the address with the master.");
  nerldProtocol.sendCommandToMaster(ADDR, 0, 0);
  Serial.println("INFO::setup: Successfully registered.");
  Serial.println("INFO::setup: Basic Slave initialised.");
}

void loop()
{
  delay(100);
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
