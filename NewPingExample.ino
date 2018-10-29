// ---------------------------------------------------------------------------
// Example NewPing library sketch that does a ping about 20 times per second.
// ---------------------------------------------------------------------------
#include "rdm630.h"
#include <NewPing.h>

#define TRIGGER_PIN  13  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     12  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define RFID_PIN 9
#define BUZZER_PIN 3

rdm630 rfid(RFID_PIN, 0);  //TX-pin of RDM630 connected to Arduino pin 6
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

unsigned long newResult = 0;
unsigned long oldResult = 0;
unsigned long prevTime = 0;

void setup() {
  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
  pinMode(BUZZER_PIN, OUTPUT);
  rfid.begin();
}

void loop() {
  /*delay(50);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  Serial.print("Ping: ");
  Serial.print(sonar.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");
  Serial.println("");*/

  byte data[6];
  byte length;

  if(rfid.available()){
    rfid.getData(data,length);
    //concatenate the bytes in the data array to one long which can be 
    //rendered as a decimal number
    unsigned long result = 
       ((unsigned long int)data[1]<<24) + 
       ((unsigned long int)data[2]<<16) + 
       ((unsigned long int)data[3]<<8) + 
       data[4];              

    oldResult = newResult;
    newResult = result;
    if(newResult != oldResult){
      Serial.println("Data valid");
      for(int i=0;i<length;i++){
        Serial.print(data[i],HEX);
        Serial.print(" ");
      }
      Serial.println();
      Serial.print("decimal CardID: ");
      Serial.println(result);
      prevTime = millis();
      digitalWrite(BUZZER_PIN, HIGH);
      delay(100);
      digitalWrite(BUZZER_PIN, LOW);
    }
  }

  if((millis() - prevTime) > 1000){
    newResult = 0;
  }
}

