#include <SPI.h> //library allows you to communicate with SPI (What is SPI H?
			Serial Peripheral Interface (SPI) is a synchronous serial data protocol used by microcontrollers for communicating with one or more 			peripheral devices quickly over short distances.) devices
#include <MFRC522.h> //Read and write different types of Radio-Frequency IDentification (RFID) cards on your Arduino using a RC522 based reader connected 			via the Serial Peripheral Interface (SPI) interface.
#include <Servo.h> //allows an Arduino board to control RC servo motors

int servoPin = 9;
Servo servo;
int angle = 0;

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
    servo.attach(servoPin);

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
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "BD 31 15 2B") // the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    servo.write(180);
    delay(3000);
  }
 
 else   {
    Serial.println(" Access denied");
    servo.write(90);
    delay(3000);
  }
}
