#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);

Servo lockServo;

String authorizedUID = "74 EB 47 A3";   // CHANGE THIS according to rfid tag id

void setup() {
  Serial.begin(9600);

  SPI.begin();
  rfid.PCD_Init();

  lockServo.attach(3);
  lockServo.write(0); // locked position

  Serial.println("Scan RFID Card");
}

void loop() {

  // Check for card
  if (!rfid.PICC_IsNewCardPresent())
    return;

  if (!rfid.PICC_ReadCardSerial())
    return;

  String cardUID = "";

  for (byte i = 0; i < rfid.uid.size; i++) {
    cardUID += String(rfid.uid.uidByte[i], HEX);
    if (i != rfid.uid.size - 1)
      cardUID += " ";
  }

  cardUID.toUpperCase();

  Serial.print("UID: ");
  Serial.println(cardUID);

  // Compare UID
  if (cardUID == authorizedUID) {

    Serial.println("Access Granted");

    lockServo.write(90); // open lock
    delay(3000);

    lockServo.write(0); // close lock
  }
  else {

    Serial.println("Access Denied");
  }

  rfid.PICC_HaltA();
}