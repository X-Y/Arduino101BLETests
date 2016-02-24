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
#include <CurieBle.h>

BLEPeripheral blePeripheral;  // BLE Peripheral Device (the board you're programming)
BLEService tamService("361dbb0c-0193-49dd-93af-753ab760a344"); // BLE LED Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLECharacteristic foodChari("6ba3791d-bc31-4c7b-8a56-df1642fb698d", BLERead | BLENotify, 2);
BLECharacteristic playChari("6ba3791d-bc31-4c7b-8a56-df1642fb698e", BLERead | BLENotify, 2);
BLECharacteristic cleanChari("6ba3791d-bc31-4c7b-8a56-df1642fb698f", BLERead | BLENotify, 2);
BLECharacteristic msgChari("6ba3791d-bc31-4c7b-8a56-df1642fb6990", BLERead | BLENotify, 20);


const int ledPin = 13; // pin to use for the LED
int foodLv,playLv,cleanLv;
long lastTimer;
bool updated;

void setup() {
  Serial.begin(9600);

  // set LED pin to output mode
  pinMode(ledPin, OUTPUT);

  // set advertised local name and service UUID:
  blePeripheral.setLocalName("Tamagotchi");
  blePeripheral.setAdvertisedServiceUuid(tamService.uuid());

  // add service and characteristic:
  blePeripheral.addAttribute(tamService);
  blePeripheral.addAttribute(foodChari);
  blePeripheral.addAttribute(playChari);
  blePeripheral.addAttribute(cleanChari);
  blePeripheral.addAttribute(msgChari);

  // set the initial value for the characeristic:
  //switchCharacteristic.setValue(0);
  unsigned char msg[]="Hello";
  msgChari.setValue(msg,3);
  
  // begin advertising BLE service:
  blePeripheral.begin();

  Serial.println("BLE LED Peripheral");

  foodLv=100;
  playLv=100;
  cleanLv=100;
  lastTimer=millis();
}

void loop() {
  // listen for BLE peripherals to connect:
  BLECentral central = blePeripheral.central();

  updateStatus();
  
  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    updated=false;
    // while the central is still connected to peripheral:
    while (central.connected()) {
      updateStatus();
      if(updated){
        Serial.println("Sending data");
        const unsigned char foodChariArr[2]={0, (char)foodLv};
        foodChari.setValue(foodChariArr,2);
        const unsigned char playChariArr[2]={0, (char)playLv};
        playChari.setValue(playChariArr,2);
        const unsigned char cleanChariArr[2]={0, (char)cleanLv};
        cleanChari.setValue(cleanChariArr,2);        
        
        updated=false;
      }
/*      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (switchCharacteristic.written()) {
        if (switchCharacteristic.value()) {   // any value other than 0
          Serial.println("LED on");
          digitalWrite(ledPin, HIGH);         // will turn the LED on
        } else {                              // a 0 value
          Serial.println(F("LED off"));
          digitalWrite(ledPin, LOW);          // will turn the LED off
        }
      }*/
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}

void updateStatus(){
  if(millis()-lastTimer>=3000){
    if(foodLv>0){
      foodLv=foodLv-1;
    }else{
      foodLv=0;
    }
    
    if(playLv>0){
      playLv=playLv-2;
    }else{
      playLv=0;
    }
    if(cleanLv>0){
      cleanLv=cleanLv-3;
    }else{
      cleanLv=0;
    }
    updated=true;
    Serial.print(foodLv);
    Serial.print(' ');
    Serial.print(playLv);
    Serial.print(' ');
    Serial.print(cleanLv);
    Serial.print(' ');
    Serial.println();
    lastTimer=millis();
  }
}


