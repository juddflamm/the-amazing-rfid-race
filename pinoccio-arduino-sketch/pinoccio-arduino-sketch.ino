#include <SPI.h>
#include <Wire.h>
#include <Scout.h>
#include <GS.h>
#include <bitlash.h>
#include <lwm.h>
#include <js0n.h>

#include <peripherals/halRgbLed.h> -I
#include <Shell.h>

#include "version.h"

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN		2		// 
#define SS_PIN		3		//

#define LED_DURATION		2000  //The time in millis the led will stay on when a card is read

String cardId = "";
unsigned long ledOffMillis = 0;
unsigned long clearCardIdMillis = 0;

MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance

void setup() {
  Scout.setup(SKETCH_NAME, SKETCH_REVISION, SKETCH_BUILD);
  // Add custom setup code here
  
  Scout.setMode(2,OUTPUT);
  Scout.setMode(3,OUTPUT);
  SPI.begin();			// Init SPI bus
  mfrc522.PCD_Init();		// Init MFRC522

}

void loop() {
  Scout.loop();
  // Add custom loop code here
  
  // Turn off the LED if the LED_DAURATION has passed
  if (ledOffMillis != 0 && millis() >= ledOffMillis) {
    HalRgbLed().turnOff();
    ledOffMillis = 0;
  }
  
  // Clear the cardId if the LED_DURATION + 1000 millis has passed
  if (clearCardIdMillis != 0 && millis() >= clearCardIdMillis) {
    cardId = "";
    clearCardIdMillis = 0;
  }
  
  // Proceed only IF the last read cardId has been cleared
  if (cardId != "") {
    return;
  }
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Read the card's UID
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    cardId += mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ";
    cardId += String(mfrc522.uid.uidByte[i], HEX);
    cardId.trim();
  }
        
  // Turn on LED
  HalRgbLed().beccapurple();
  
  // Set timers for LED and clearing cardId
  ledOffMillis = millis() + LED_DURATION;
  clearCardIdMillis = ledOffMillis + 1000;

  // Report the rfid.read event
  String evalCommand = "a = key(\"";
  evalCommand += cardId;
  evalCommand += "\"); hq.report(\"rfid.read\", a); key.free(a);";
 
 PinoccioShell().eval(evalCommand);

}


