#define BLYNK_TEMPLATE_ID "TMPLc9OBG_DD" // defines template ID
#define BLYNK_DEVICE_NAME "IOT" // defines name of the device

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG //enable debug prints on the default Serial port

//#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD


#include <dht.h> //import DHT sensor library
dht DHT;
#define DHT11_PIN  2   //D4
#define MQ2_PIN   A0   //A0
#define pirPin    10   //SD3 




// define the GPIO connected with Relays and switches
#define RelayPin1 14   //D5
#define RelayPin2 12   //D6
#define RelayPin3 13   //D7
#define RelayPin4 15   //D8

#define BuzzPin  4   //D2
#define wifiLed  16   //D0

//Change the virtual pins according the rooms
#define VPIN_BUTTON_1    V1
#define VPIN_BUTTON_2    V2
#define VPIN_BUTTON_3    V3 
#define VPIN_BUTTON_4    V4
#define VPIN_BUTTON_5    V5

#define VPIN_BUTTON_C    V9
#define VPIN_TEMPERATURE V10
#define VPIN_HUMIDITY    V11
#define VPIN_MQ2         V12

// Relay State
bool toggleState_1 = LOW; //Define integer to remember the toggle state for relay 1
bool toggleState_2 = LOW; //Define integer to remember the toggle state for relay 2
bool toggleState_3 = LOW; //Define integer to remember the toggle state for relay 3
bool toggleState_4 = LOW; //Define integer to remember the toggle state for relay 4
bool toggleState_5 = LOW; //Define integer to remember the toggle state for relay 5

// Switch State
bool SwitchState_1 = LOW;
bool SwitchState_2 = LOW;
bool SwitchState_3 = LOW;
bool SwitchState_4 = LOW;

float temperature1 = 0; //initialize temperature value as 0
float humidity1   = 0; //initialize humidity value as 0
int   MQ2Val = 0; //initialize smoke value as 0


int statuss = 0;
int out = 0;


#include "BlynkEdgent.h" //Blynk.Edgent - a Blynk hardware agent library that manages device connection and communication with the cloud.
BlynkTimer timer2; //BlynkTimer allows you to send data periodically with given intervals not interfering with Blynk library routines 

void readSensor(){
    int chk = DHT.read11(DHT11_PIN);
  MQ2Val = analogRead(MQ2_PIN);

    humidity1 = DHT.humidity;
    temperature1 = DHT.temperature;
     

      Serial.println(MQ2Val);
      Serial.println(DHT.temperature);
      Serial.println(DHT.humidity);

}

void sendSensor()
{
  readSensor();
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(VPIN_HUMIDITY, humidity1);
  Blynk.virtualWrite(VPIN_TEMPERATURE, temperature1);
  Blynk.virtualWrite(VPIN_MQ2, MQ2Val);
}

BLYNK_CONNECTED() {                                  //Returns true when hardware is connected to Blynk Server, false if there is no active connection to Blynk server.
  // Request the latest state from the server
  Blynk.syncVirtual(VPIN_BUTTON_1);    //syncVirtual is used to act as if you pressed a button
  Blynk.syncVirtual(VPIN_BUTTON_2);
  Blynk.syncVirtual(VPIN_BUTTON_3);
  Blynk.syncVirtual(VPIN_BUTTON_4);
   Blynk.syncVirtual(VPIN_BUTTON_5);
  Blynk.syncVirtual(VPIN_TEMPERATURE);
  Blynk.syncVirtual(VPIN_HUMIDITY);
  Blynk.syncVirtual(VPIN_MQ2);
}

// When App button is pushed - switch the state

BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleState_1 = param.asInt();  //param.asInt() to interpret incoming data as Integers
  if(toggleState_1 == 1){
    digitalWrite(RelayPin1, HIGH);
  }
  else { 
    digitalWrite(RelayPin1, LOW);
  }
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  toggleState_2 = param.asInt();
  if(toggleState_2 == 1){
    digitalWrite(RelayPin2, HIGH);
  }
  else { 
    digitalWrite(RelayPin2, LOW);
  }
}

BLYNK_WRITE(VPIN_BUTTON_3) {
  toggleState_3 = param.asInt();
  if(toggleState_3 == 1){
    digitalWrite(RelayPin3, HIGH);
  }
  else { 
    digitalWrite(RelayPin3, LOW);
  }
}

BLYNK_WRITE(VPIN_BUTTON_4) {
  toggleState_4 = param.asInt();
  if(toggleState_4 == 1){
    digitalWrite(RelayPin4, HIGH);
  }
  else { 
    digitalWrite(RelayPin4, LOW);
  }
}

BLYNK_WRITE(VPIN_BUTTON_5) {
  toggleState_5 = param.asInt();
}


void setup()
{
  Serial.begin(115200); //establish serial communication. Serial communication is one way to allow devices to talk to each other. Transmits 115200 bits per 			second.
  delay(100);
  
  pinMode(RelayPin1, OUTPUT); //Set pins as input or output pins
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);
  pinMode(BuzzPin, OUTPUT);

  pinMode(wifiLed, OUTPUT);    
  pinMode(pirPin, INPUT); 
  


  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, LOW);
  digitalWrite(RelayPin2, LOW);
  digitalWrite(RelayPin3, LOW);
  digitalWrite(RelayPin4, LOW);

  digitalWrite(wifiLed, HIGH);

  BlynkEdgent.begin();

  Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
  Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
  Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
  Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
  Blynk.virtualWrite(VPIN_BUTTON_5, toggleState_5);

  
  // Setup a function to be called every 2 seconds
  timer2.setInterval(2000L, sendSensor);

}

void loop() {
    BlynkEdgent.run();

    
   if(DHT.temperature >= 33){
      digitalWrite(RelayPin1, HIGH);
      Blynk.virtualWrite(VPIN_BUTTON_1, HIGH);
      toggleState_1 = HIGH;
      SwitchState_1 = HIGH;
      Serial.println("Switch-1 on");
  }
  else{ 
      digitalWrite(RelayPin1, LOW);
      Blynk.virtualWrite(VPIN_BUTTON_1, LOW);
      toggleState_1 = LOW;
      SwitchState_1 = LOW;
      Serial.println("Switch-1 off");  
  }


    if(digitalRead(pirPin) == HIGH && toggleState_5 == 1){
      
      digitalWrite(RelayPin2, HIGH);
      Blynk.virtualWrite(VPIN_BUTTON_2, HIGH);
      toggleState_2 = HIGH;
      SwitchState_2 = HIGH;
      Serial.println("Switch-2 on");
  }
  else if(digitalRead(pirPin) == LOW && toggleState_5 == 1){  
      digitalWrite(RelayPin2, LOW);
      Blynk.virtualWrite(VPIN_BUTTON_2, LOW);
      toggleState_2 = LOW;
      SwitchState_2 = 0;
      Serial.println("Switch-2 off");
  }

    
    if( MQ2Val >= 50){
      digitalWrite(BuzzPin, HIGH);
  }
  else{ 
      digitalWrite(BuzzPin, LOW);
  }


timer2.run();
}
