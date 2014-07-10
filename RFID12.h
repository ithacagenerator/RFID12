#include <stdint.h>

#define RFID12_CODE_LENGTH 6

#define RFID12_STATE_WAITING_FOR_HEADER 0
#define RFID12_STATE_WAITING_FOR_12_BYTES 1
#define RFID12_STATE_WAITING_FOR_CALLER_TO_READ 2

class RFID12{
private:
  uint8_t code[RFID12_CODE_LENGTH];
  uint8_t checksum;
  uint8_t bytesread;
  uint8_t tempbyte;
  uint8_t state;
  
  void initStateVariables(void);
public:
  RFID12(void);
  uint8_t * getTag(void);
  uint8_t process(uint8_t val);
  uint8_t getChecksum(void);
  void nextTag(void);
};


