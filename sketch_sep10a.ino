// Required Libraries
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// Circuit Pinout
#define BUZZER 15
#define ESTETUINO 

// Global Variables
int mode = 1;
int hasBuzzed = 1;
int hasSent = 1;
const char *ssid = "BunkerDeBlacky";
const char *pass = "FE80BLACKY";
WiFiClient client;

void setup_wifi(){
  Serial.println("Conecting to: ");
  Serial.print(ssid);
  Serial.print("\n");
  WiFi.begin(ssid,pass);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  Serial.println("IP Address:");
  Serial.print(WiFi.localIP());
  Serial.print("\n");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // Component Initialization
  pinMode(BUZZER,OUTPUT);
  //Wireless initialization
  setup_wifi();
}

/**
 * Makes a small sound
 */
void buzz(int m){
  for(int i=0;i<m;i++){
    tone(BUZZER,1000);
    delay(200);
    noTone(BUZZER);
    delay(200);
  }
}

void loop() {
  if(!--hasBuzzed){
    buzz(mode);
  }

  if(WiFi.status() == WL_CONNECTED && !--hasSent){
    HTTPClient http;
    http.begin("http://192.168.0.5:3000/hello");
    http.addHeader("Content-Type","application/json");
    //
    StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
    JsonObject& JSONencoder = JSONbuffer.createObject(); 
 
    JSONencoder["sensorType"] = "Temperature";
 
    JsonArray& values = JSONencoder.createNestedArray("values"); //JSON array
    values.add(20); //Add value to array
    values.add(21); //Add value to array
    values.add(23); //Add value to array
    char JSONmessageBuffer[300];
    JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println(JSONmessageBuffer);
    //
    int httpCode = http.POST(JSONmessageBuffer);
    String payload = http.getString();                
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload
 
    http.end();  //Close connection
  }
}
