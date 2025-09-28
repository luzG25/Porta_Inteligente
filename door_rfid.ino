// Programa : RFID - Controle de Acesso com Servo
// Autor    : Gabriel da Luz + ajustes

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>          // Biblioteca para o servo

#define SS_PIN 10
#define RST_PIN 9
#define SERVO_PIN 8         // Pino do servo

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Instância do MFRC522
Servo trava;                        // Instância do Servo

// UID autorizados – use sempre letras maiúsculas e mesmo formato
const String UID_CHAVEIRO = "F9 6E 4F 05";
const String UID_CARTAO   = "BD 9B 06 7D";

const int ANG_INIT = 150;
const int ANG_FIN = 40;

void setup() 
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  Serial.println("Aproxime//Programa : RFID - Controle de Acesso Modulo RFID com Arduino
//Autor : Gabriel da Luz

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.


char st[20];

void setup() 
{
  Serial.begin(9600); // Inicia a serial
  SPI.begin();    // Inicia  SPI bus
  mfrc522.PCD_Init(); // Inicia MFRC522
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
}

void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();

  Serial.print("Codigo do RFID: ");
  Serial.print(conteudo);
  
  if (conteudo.substring(1) == "F9 6E 4F 05") //UID 1 - Chaveiro
  {
    Serial.println("Ola MakerHero !");
    Serial.println();

    delay(3000);
 
  }

  if (conteudo.substring(1) == "BD 9B 06 7D") //UID 2 - Cartao
  {
    Serial.println("Ola Cartao !");
    Serial.println();
    
    delay(3000);
  }
} 
 o seu cartao do leitor...");
  Serial.println();

  // Inicializa o servo
  trava.attach(SERVO_PIN);
  trava.write(ANG_INIT); // posição inicial fechada (ajuste conforme seu mecanismo)
}

void loop() 
{
  // Verifica se existe um novo cartão
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial())   return;

  // Monta a string do UID lido
  String conteudo = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) conteudo += " 0";
    else                               conteudo += " ";
    conteudo += String(mfrc522.uid.uidByte[i], HEX);
  }
  conteudo.toUpperCase();
  Serial.print("UID lido:");
  Serial.println(conteudo);

  // Remove o primeiro espaço para comparar com suas UIDs cadastradas
  String uid = conteudo.substring(1);
  Serial.print("Codigo do RFID: ");
  Serial.println(uid);

  // --- Autorização ---
  if (uid == UID_CHAVEIRO || uid == UID_CARTAO) {
    Serial.println("Acesso autorizado!");
    abrirTrava();          // Aciona o servo
  } else {
    Serial.println("Acesso negado!");
  }

  // Para evitar leituras múltiplas seguidas
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

// Função para girar o servo
void abrirTrava() {
  trava.write(ANG_FIN);      // abre (ajuste o ângulo conforme seu hardware)
  delay(3000);          // tempo que a porta fica aberta
  trava.write(ANG_INIT);       // fecha novamente
  Serial.println("Trava fechada.");
}