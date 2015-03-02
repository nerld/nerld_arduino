// Wire Slave Receiver

#include <Wire.h>


int DEFAULT_ADDR = 99;
int ADDR = NULL;

int DELAY = 5000; // delay between each measurement

int LED = 10; // LED pin to show the status of the slave

int sensorPin = 0; //the analog pin the TMP36's Vout (sense) pin is connected to
                        //the resolution is 10 mV / degree centigrade with a
                        //500 mV offset to allow for negative temperatures


void setup()
{
  Serial.begin(9600); 
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  
  Serial.println("Starting TMP36");

  Wire.begin(DEFAULT_ADDR);
  Wire.onRequest(setDelay);
  Wire.requestFrom(1,1);
  while (ADDR == NULL && Wire.available()) {
    ADDR = (Wire.read());
    delay(100);
    TWAR = ADDR << 1;
  }

   //send addr + func+type
  String command = "00";
  String type = "3";
  String address = "0";
  if (ADDR < 10) {
    address = address + ADDR;
  } else {
    address = "";
    address = address + ADDR;
  }
  String commandString = command + address;
  commandString = commandString  + type; 
  Serial.println(commandString);
  char buffer[6];
  commandString.toCharArray(buffer, 6);
  Serial.println(buffer);
  Wire.beginTransmission(1);
  Wire.write(buffer);
  Wire.endTransmission();
}

void loop()
{
  measure();
  delay(DELAY);                                     //waiting a second
}

void measure() {
  
  digitalWrite(LED, HIGH);
  int reading = analogRead(sensorPin);  
 
  // converting that reading to voltage, for 3.3v arduino use 3.3
  float voltage = reading * 5.0;
  voltage /= 1024.0; 

  // print out the voltage
  Serial.print(voltage); Serial.println(" volts");

  // now print out the temperature
  float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((voltage - 500mV) times 100)
  Serial.print(temperatureC); Serial.println(" degrees C");

  digitalWrite(LED, LOW);

  // now convert to Fahrenheit
  // float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
  // Serial.print(temperatureF); Serial.println(" degrees F");
  
  int tempInt = temperatureC *100;
  String command = "01";
//  String type = "3";
  String address = "0";
  
//  char tempStr[5];
//  dtostrf(temperatureC*100, 5, 2, tempStr);
//  String temp = String(tempStr);
//  
  
  if (ADDR < 10) {
    address = address + ADDR;
   } else {
     address = "";
     address = address + ADDR;
   }
   String commandString = command + address;
   commandString = commandString + tempInt; 
   Serial.println(commandString);
   char buffer[11];
   commandString.toCharArray(buffer, 11);
   Serial.println(buffer);
   Wire.beginTransmission(1);
   Wire.write(buffer);
   Wire.endTransmission();

   
}

void setDelay() {
  Serial.print("set delay called");
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()

