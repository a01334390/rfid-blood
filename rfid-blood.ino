/**
      __    _____    _   __  __ _ __
    / / _ /_  _/   / \,' /,'_//// /
   / /,'o| / /___ / \,' // /_/ U /
   /_/ |_,'/_//__//_/ /_/ |__/\_,'
   Made by: Fernando Martin Garcia Del Ange
   Built on: September 13th, 2019
*/

// Required Libraries
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_GFX.h>
#include "Max72xxPanel.h"
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
#define LOCATION 115

// Global Variables
int mode = 1;
int hasBuzzed = 1;
int hasSent = 1;
// Internet Variables
const char *ssid = "BunkerDeBlacky";
const char *pass = "FE80BLACKY";
WiFiClient client;
ESP8266WebServer server(80);
//RFID Variables
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
byte nuidPICC[4];
//Blood Bag PICC ID
String bloodbag = "";

/**
   Starts up the RFID Scanner
*/
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

/**
   Connects to WiFi and Creates a Client on port 80
*/
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
  server.on("/", handle_onConnect);
  server.onNotFound(handle_onNotFound);
  server.begin();
  Serial.println("==== HTTP Server began ====");
  delay(1000);
}

/**
   Setup procedure
*/
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
   Serves / path
*/
void handle_onConnect() {
  server.send(200, "text/html", bloodbag_read());
}

/**
   Serves any other path
*/
void handle_onNotFound() {
  server.send(404, "text/plain", "Not Found");
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

/**
   Helper routine to make the buzzer make a confirmation or error sound
   1 => OK, 0 => NOT OK
*/
void confirmation_buzz(int res) {
  if (res) {
    tone(BUZZER, 500);
    delay(200);
    tone(BUZZER, 1000);
    delay(100);
    noTone(BUZZER);
    delay(200);
  } else {
    tone(BUZZER, 1000);
    delay(100);
    tone(BUZZER, 500);
    delay(100);
    noTone(BUZZER);
    delay(200);
  }
}

/**
   Main Program Loop
*/
void loop() {
  // Buzz for mode
  if (!--hasBuzzed) {
    buzz(mode);
  }

  //Main Switch
  switch (mode) {
    case 1:
      server.handleClient();
      rfid_read();
      break;
    case 2:
      rfid_read();
      break;
  }
}

/**
   POSTs to the Blockchain Server
*/
void checkpoint_send() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String postcode = "http://3.222.166.83/blood/move/" + bloodbag + "/" + LOCATION + "/user1";
    http.begin(postcode);
    int httpCode = http.POST("");
    String payload = http.getString();
    Serial.println(httpCode);
    Serial.println(payload);
    http.end();
  }
}

/**
   Reads the ID from the MiFare Tag
*/
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
    confirmation_buzz(0);
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
    Serial.println("Card ID: " + bloodbag);
    confirmation_buzz(1);
    if (mode == 2) {
      checkpoint_send();
    }
  } else {
    Serial.println(F("Card read previously."));
    confirmation_buzz(0);
  }
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
  bloodbag = "";
  for (byte i = 0; i < bufferSize; i++) {
    if (i == 0) {
      bloodbag += "";
    } else {
      bloodbag += buffer[i] < 0x10 ? " 0" : "-";
    }
    bloodbag += buffer[i];
  }
}

/**
   Returns the webpage to serve to the server
*/
String bloodbag_read() {
  String ptr = "<!DOCTYPE html>\n";
  ptr += "<html>\n";
  ptr += "<head>\n";
  ptr += "<title> Right Donor's RFID Prototype </title>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>Blood Bag ID</h1>\n";
  if (bloodbag != "")
    ptr += "<h4>The Blood Bag ID is: " + bloodbag + "</h4>";
  else
    ptr += "<h4> Put a new tag on the reader <h4>";
  ptr += "<p><a href=\"/\"><button class=\"button\">Reload</button></a></p>";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
