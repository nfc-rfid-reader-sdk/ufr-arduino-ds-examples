#include <uFR.h>

#define SERIAL_MONITOR
//#define DEBUG_CODE


// If this doesn't work, try switching rx and tx
uFR reader(10, 11, 12); // rx, tx, reset



void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
#ifdef SERIAL_MONITOR
  Serial.begin(115200);
  Serial.print("\n115200 bps set\n");
#endif  
  reader.begin();
  delay(2000); // Required!
  
  /*  For testing purposes:
   *  reader.setRedLED(HIGH);
   *  delay(2000);
   *  reader.setRedLED(HIGH);
   */
  //uint8_t type[4];
}
void loop() {
  uint8_t cardID[10];
  uint8_t card_type = 0;
  uint8_t length = 0;
  uint8_t code;
  uint8_t aesKEY[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//  uint8_t aesKEY[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
//  uint8_t aesKEY[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
//  uint8_t aesKEY[16] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  code = reader.getCardID(cardID, &length, &card_type);
  //Serial.print(code);
  if (code == 0)
  {
    card_type = 0;
    code = reader.getCardTypeDLogic(&card_type);
    if(card_type <= 0x27 || card_type >= 0x2D)
    {
#ifdef SERIAL_MONITOR     
      Serial.print("Card is not Desfire!\nCard Type: ");
      Serial.print(card_type, HEX);
#endif
    }
    else
    {
      code = reader.getDesfireUID(cardID, &length, 0, 0xF00001, 1); //1st par cardID, 2nd par cardID Length, 3th par AIS Key number in reader, 4th par Aplication ID, 5th par Aplication ID Key Index
      //code = reader.getDesfireUIDPK(cardID, &length, aesKEY, 0xF00001, 1); //1st par cardID, 2nd par AES Key that we provide to reader, 3th par Aplication ID, 5th par Aplication ID Key Index
#ifdef SERIAL_MONITOR
      if (code == 0)
      {
        Serial.print("Desfire card detected.\nUID: ");
        for(int i=0;i<length;++i)
        {
          if(cardID[i]<16)
          {
            Serial.print("0");
          }
          Serial.print(cardID[i], HEX);
          Serial.print(" ");
        }
        Serial.print("\n");
      }
      else
      {
          Serial.print("Error geting Desfire UID!\n");
      }
#endif
    }
   }

  // Reduce delay if response is slow
  delay(200);
}
