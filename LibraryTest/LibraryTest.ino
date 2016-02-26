#include <CapacitiveSensor.h>
#include <Servo.h>
#include <SPI.h>
#include <SD.h>
#include <EducationShield.h>
#include <CurieBle.h>

BLEText messenger;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  messenger.setName("Msg");
  messenger.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(messenger.searchCentral()){
    Serial.println("Connected to central: ");
    while(messenger.connected()){
      if(messenger.dataReceived()){
        for(int i=0;i<messenger.MAX_LENGTH && messenger.getCharAt(i)!='\0';i++){
          Serial.print(messenger.getCharAt(i));
        }
        Serial.println();

      }
    }
    Serial.println(F("Disconnected from central: "));

  }
}
