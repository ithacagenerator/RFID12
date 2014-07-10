#include "RFID12.h"
RFID12 rfid;
#include <string.h>

#define NUM_KNOWN_TAGS 3
#define BYTES_PER_TAG_ID 5
uint8_t known_tags[NUM_KNOWN_TAGS][BYTES_PER_TAG_ID] = {
  {0x15, 0x73, 0x11, 0x02, 0x55},
  {0x00, 0x00, 0x0e, 0xd7, 0xb4},
  {0x15, 0x73, 0x11, 0x02, 0x55}
};

void blinkFast(){
  for(int ii = 0; ii < 5; ii++){
    digitalWrite(13, HIGH);
    delay(50);
    digitalWrite(13, LOW);
    delay(50); 
  }
}

void blinkSlow(){
  for(int ii = 0; ii < 2; ii++){
    digitalWrite(13, HIGH);
    delay(250);
    digitalWrite(13, LOW);
    delay(250); 
  }  
}

void setup() {
  Serial.begin(9600);  // connect to the serial port
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW); 
  pinMode(2, OUTPUT); // rfid reset pin
  digitalWrite(2, HIGH);
}

void loop(){
    char ch = 0;
    uint8_t * tag;
    if(Serial.available()){
      ch = Serial.read();
      if(rfid.process(ch)){
         tag = rfid.getTag();
         int8_t idx = findTag(tag);
         if(idx > 0){
           validTagResponse(idx);
         }
         else{
           invalidTagResponse(tag);
         }
         
         rfid.nextTag();
       }
    }
}

int8_t findTag(uint8_t * the_tag){
  for(uint8_t ii = 0; ii < NUM_KNOWN_TAGS; ii++){
    if(memcmp(the_tag, known_tags[ii], 5) == 0){
        return ii;
    }
  }
  
  return -1;
}

// BEHAVIOR CODE BELOW HERE

void validTagResponse(uint8_t index){
    uint8_t * tag = known_tags[index];
    Serial.print("Valid 5-byte code: ");
    for (uint8_t i=0; i<5; i++) {
      if (tag[i] < 16) Serial.print("0");
      Serial.print(tag[i], HEX);
      Serial.print(" ");
    }
    Serial.println(); 
    
    blinkFast();
}

void invalidTagResponse(uint8_t * tag){
    Serial.print("Invalid 5-byte code: ");
    for (uint8_t i=0; i<5; i++) {
      if (tag[i] < 16) Serial.print("0");
      Serial.print(tag[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
    
    blinkSlow();
}

