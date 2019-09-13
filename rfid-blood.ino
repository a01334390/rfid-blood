// Required Libraries
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>

/* ----------------------------------
               MFRC522      Node
               Reader/PCD   MCU
   Signal      Pin          Pin
   ----------------------------------
   RST/Reset   RST          D1 (GPIO5)
   SPI SS      SDA(SS)      D2 (GPIO4)
   SPI MOSI    MOSI         D7 (GPIO13)
   SPI MISO    MISO         D6 (GPIO12)
   SPI SCK     SCK          D5 (GPIO14)
   3.3V        3.3V         3.3V
   GND         GND          GND
*/

#define RST_PIN 5
#define SS_PIN 4
#define BUZZER 15
#define ESTETUINO

// Global Variables
int mode = 1;
int hasBuzzed = 1;
int hasSent = 1;
const char *ssid = "BunkerDeBlacky";
const char *pass = "FE80BLACKY";
WiFiClient client;
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
byte nuidPICC[4];
String bloodbag = "";

void setup_rfid() {
  SPI.begin();
  rfid.PCD_Init();
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  Serial.println(F("=== RFID Scanner is ready ===="));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
}

void setup_wifi() {
  Serial.println("Conecting to: ");
  Serial.print(ssid);
  Serial.print("\n");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n==== WiFi Connected! ====");
  Serial.println("IP Address:");
  Serial.print(WiFi.localIP());
  Serial.print("\n");
  buzz(1);
  delay(1000);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // Component Initialization
  pinMode(BUZZER, OUTPUT);
  //Wireless initialization
  setup_wifi();
  setup_rfid();
}

/**
   Helper routine to make the buzzer make a sound
*/
void buzz(int m) {
  for (int i = 0; i < m; i++) {
    tone(BUZZER, 1000);
    delay(200);
    noTone(BUZZER);
    delay(200);
  }
}

void loop() {
  // Buzz for mode
  if (!--hasBuzzed) {
    buzz(mode);
  }

  switch (mode) {
    case 1:
      rfid_read();
      break;
    case 2:
      break;
  }

  //  if(WiFi.status() == WL_CONNECTED && !--hasSent){
  //    HTTPClient http;
  //    http.begin("http://192.168.0.5:3000/hello");
  //    http.addHeader("Content-Type","application/json");
  //    //
  //    StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
  //    JsonObject& JSONencoder = JSONbuffer.createObject();
  //
  //    JSONencoder["sensorType"] = "Temperature";
  //
  //    JsonArray& values = JSONencoder.createNestedArray("values"); //JSON array
  //    values.add(20); //Add value to array
  //    values.add(21); //Add value to array
  //    values.add(23); //Add value to array
  //    char JSONmessageBuffer[300];
  //    JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  //    Serial.println(JSONmessageBuffer);
  //    //
  //    int httpCode = http.POST(JSONmessageBuffer);
  //    String payload = http.getString();
  //    Serial.println(httpCode);   //Print HTTP return code
  //    Serial.println(payload);    //Print request response payload
  //
  //    http.end();  //Close connection
  //  }
}

void rfid_read() {
  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  if (rfid.uid.uidByte[0] != nuidPICC[0] ||
      rfid.uid.uidByte[1] != nuidPICC[1] ||
      rfid.uid.uidByte[2] != nuidPICC[2] ||
      rfid.uid.uidByte[3] != nuidPICC[3] ) {
    Serial.println(F("A new card has been detected."));

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }

    decode_id(rfid.uid.uidByte, rfid.uid.size);
    Serial.println("Card ID: "+bloodbag);
  }
  else Serial.println(F("Card read previously."));
  // Halt PICC
  rfid.PICC_HaltA();
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();

}

/**
   Helper routine to dump a byte array as hex values to Serial.
*/
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
   Helper routine to dump a byte array as dec values to Serial.
*/
void decode_id(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    bloodbag+=buffer[i] < 0x10 ? " 0" : " ";
    bloodbag+=buffer[i];
  }
}
