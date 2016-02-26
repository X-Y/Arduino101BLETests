#include <CapacitiveSensor.h>
#include <Servo.h>
#include <SPI.h>
#include <SD.h>
#include <EducationShield.h>
#include <CurieBle.h>

BLEuart uart;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  uart.setName("UART");
  uart.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(uart.searchCentral()){
    Serial.println("Connected to central: ");
    while(uart.connected()){
      
      if(uart.dataReceived()){
        uart.fetchData();
        Serial.println(uart.receivedString());
        uart.sendString(uart.receivedString(),20);
      }
      /*
      uart.sendString("I want icecream",15);
      delay(1000);
      */
      /*
      uart.addValue(analogRead(A0));
      uart.addValue(analogRead(A1));
      uart.addValue(analogRead(A2));
      uart.send();
      delay(1000);
      */
      /*
      if(uart.dataReceived()){
        uart.fetchData();
        Serial.println(uart.getValueAt(0));
      }
      */
    }
    Serial.println(F("Disconnected from central: "));

  }
}
