#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "max6675.h"
 
const char* ssid = "SSID";
const char* password = "WPAKEY";

//Your Domain name with URL path or IP address with path
String serverName = "http://YOUR.DOMAIN.NAME/metrics";

int thermoDO = 12; // D6 at esp8266
int thermoCS = 13; // D7
int thermoCLK = 14; // D5

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

int ledPin = 2; // D4 pin
WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 
  // Mit Wifi verbinden
  Serial.print("Verbinden mit: "); 
  Serial.println(ssid);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi verbunden");
 
  // Start des Servers
  server.begin();
  Serial.println("Server gestartet");
  
}

       

void loop() {
 
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = serverName + "?device=temperature&temperature=" + thermocouple.readCelsius();
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
 
  }
 
  delay(5000);    //Send a request every 30 seconds
}
