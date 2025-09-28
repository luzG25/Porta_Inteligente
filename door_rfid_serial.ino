#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN     10
#define RST_PIN     9
#define SERVO_PIN   8

MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo trava;

const int ANG_INIT = 150;   // fechado
const int ANG_FIN  = 40;    // aberto

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  trava.attach(SERVO_PIN);
  trava.write(ANG_INIT);

  Serial.println("Arduino pronto. Aproxime o cartão.");
}

void loop() {
  // 1️⃣  Lê novo cartão e envia UID ao Node
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      if (mfrc522.uid.uidByte[i] < 0x10) uid += "0";
      uid += String(mfrc522.uid.uidByte[i], HEX);
      if (i < mfrc522.uid.size - 1) uid += " ";
    }
    uid.toUpperCase();
    Serial.print("UID:");
    Serial.println(uid);     // <- Node.js escuta esta linha

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }

  // 2️⃣  Escuta comando do Node
  if (Serial.available() > 0) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd.equalsIgnoreCase("OPEN")) {
      abrirTrava();
      Serial.println("ACK:OPENED"); // opcional: confirma ao Node
    }
  }
}

void abrirTrava() {
  trava.write(ANG_FIN);
  delay(3000);
  trava.write(ANG_INIT);
  Serial.println("Trava fechada.");
}
