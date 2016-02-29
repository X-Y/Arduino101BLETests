#include <CapacitiveSensor.h>
#include <Servo.h>
#include <SPI.h>
#include <SD.h>
#include <EducationShield.h>
#include <CurieBle.h>


BLEuart uart=BLEuart(SERVICEID_TAMAGOTCHI);

const int ledPin = 13; // pin to use for the LED
int foodLv,playLv,cleanLv;
long lastTimer;
bool updated;

void setup() {
  Serial.begin(9600);

  // set LED pin to output mode
  pinMode(ledPin, OUTPUT);

  uart.setName("Tamagotchi");
  uart.begin();

  foodLv=100;
  playLv=100;
  cleanLv=100;
  lastTimer=millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  updateStatus();
  
  if(uart.searchCentral()){
    Serial.println("Connected to central ");
    while(uart.connected()){
      updateStatus();
      if(updated){
        Serial.println("Sending data");
        uart.addValue((unsigned char)foodLv);
        uart.addValue((unsigned char)playLv);
        uart.addValue((unsigned char)cleanLv);    
        uart.send();
        
        updated=false;
      }
    }
    Serial.println(F("Disconnected from central "));

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


