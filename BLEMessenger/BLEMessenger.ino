#include <CapacitiveSensor.h>
#include <Servo.h>
#include <SPI.h>
#include <SD.h>
#include <EducationShield.h>
#include <CurieBle.h>

BLEuart uart=BLEuart(SERVICEID_MESSENGER);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  uart.setName("UART");
  uart.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(uart.searchCentral()){
    Serial.println("Connected to central ");
    while(uart.connected()){

      //When receiving data from mobile app, write it 
      //back to the sender
      if(uart.dataReceived()){
        uart.fetchData();
        Serial.println(uart.receivedString());
        uart.sendString(uart.receivedString(),20);
      }

    }
    Serial.println(F("Disconnected from central "));

  }
}
