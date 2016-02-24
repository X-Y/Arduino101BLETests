/*
   Copyright (c) 2015 Intel Corporation.  All rights reserved.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <CapacitiveSensor.h>
#include <SPI.h>
#include <SD.h>
#include <EducationShield.h>

//Servo.h is necessary to be included here
#include <Servo.h>

#include <CurieBle.h>

BLEPeripheral blePeripheral;  // BLE Peripheral Device (the board you're programming)
BLEService logoService("19f82bd2-da79-11e5-b5d2-0a1d41d68578"); // BLE LED Service

BLECharacteristic logoCharacteristic("19f82bd2-da79-11e5-b5d2-0a1d41d68579", BLERead | BLEWrite, 20);

const int ledPin = 13; // pin to use for the LED
unsigned char charBuffer[20];

Wheels wheels=Wheels(6, 9);

void setup() {
  Serial.begin(9600);

  wheels.begin();

  // set advertised local name and service UUID:
  blePeripheral.setLocalName("LogoRobot");
  blePeripheral.setAdvertisedServiceUuid(logoService.uuid());

  // add service and characteristic:
  blePeripheral.addAttribute(logoService);
  blePeripheral.addAttribute(logoCharacteristic);

  // begin advertising BLE service:
  blePeripheral.begin();

  Serial.println("BLE LED Peripheral");
}

void loop() {
  // listen for BLE peripherals to connect:
  BLECentral central = blePeripheral.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (logoCharacteristic.written()) {
        //int bufferSize=sizeof switchCharacteristic.value();
        //Serial.println(bufferSize);
        memcpy(charBuffer,logoCharacteristic.value(),20);
        for(int i=0;i<20 && charBuffer[i]!=0;i++){
          unsigned char n=charBuffer[i];
          
          Serial.print(n);
          logoMove(n);
        }
        Serial.println();
        
        //Serial.println(switchCharacteristic.value());
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}


void logoMove(unsigned char direction){
  switch(direction){
    case 49:
      wheels.goForward();
      delay(3000);
      break;
    case 50:
      wheels.goBackwards();
      delay(3000);
      break;
    case 51:
      wheels.turnLeft();
      delay(1500);
      break;
    case 52:
      wheels.turnRight();
      delay(1500);
      break;
  }
  wheels.standStill();
  delay(300);
}


