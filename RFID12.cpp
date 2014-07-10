#include "RFID12.h"

RFID12::RFID12(void){
  initStateVariables();
}

void RFID12::initStateVariables(void){
  for(uint8_t ii = 0; ii < RFID12_CODE_LENGTH; ii++){
    code[ii] = 0;
  } 
  
  checksum = 0;
  bytesread = 0;
  tempbyte = 0;
  state = RFID12_STATE_WAITING_FOR_HEADER;    
}

uint8_t * RFID12::getTag(void){
  return code;
}

uint8_t RFID12::process(uint8_t val){
    uint8_t tag_available = false;
    switch(state){
    case RFID12_STATE_WAITING_FOR_HEADER:
      initStateVariables();
      if( val == 2 ){
        state = RFID12_STATE_WAITING_FOR_12_BYTES;
      }
      break;
    case RFID12_STATE_WAITING_FOR_12_BYTES:
    
      if((val == 0x0D)||(val == 0x0A)||(val == 0x03)||(val == 0x02)) { // if header or stop bytes before the 10 digit reading 
        state = RFID12_STATE_WAITING_FOR_HEADER;           // stop reading
        break;
      }
      
      // Do Ascii/Hex conversion:
      if ((val >= '0') && (val <= '9')) {
        val = val - '0';
      } else if ((val >= 'A') && (val <= 'F')) {
        val = 10 + val - 'A';
      }      

      // Every two hex-digits, add byte to code:
      if (bytesread & 1 == 1) {
        // make some space for this hex-digit by
        // shifting the previous hex-digit with 4 bits to the left:
        code[bytesread >> 1] = (val | (tempbyte << 4));

        if (bytesread >> 1 != 5) {                // If we're at the checksum byte,
          checksum ^= code[bytesread >> 1];       // Calculate the checksum... (XOR)
        };
      } else {
        tempbyte = val;                           // Store the first hex digit first...
      };

      bytesread++;                                // ready to read next digit
    
      if(bytesread == 12){
         if(code[5] == checksum){
           state = RFID12_STATE_WAITING_FOR_CALLER_TO_READ;  
           tag_available = true;
         }
         else{
           state = RFID12_STATE_WAITING_FOR_HEADER;
         }
      }
      
      break;
    case RFID12_STATE_WAITING_FOR_CALLER_TO_READ:
      tag_available = true;
      break;
    default:
      break;
    }
    
    return tag_available;
}

uint8_t RFID12::getChecksum(void){
  return checksum;
}

void RFID12::nextTag(void){
  initStateVariables();
}

