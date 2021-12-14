/*********
  Rui Santos
  Complete instructions at https://RandomNerdTutorials.com/esp32-web-server-gauges/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  This is a modified version of the original code used to display indoor and outdoor readings simultaneously.  It has been modified by 
  Tom Wilson, Brasilito, Costa Rica and is made available publicly under the same conditions as above.
  
*********/

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>
#include "DHT.h"


// Replace with your network credentials
const char* ssid = "MY SSID";
const char* password = "MY PASSWORD";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

// Json Variable to Hold Sensor Readings
JSONVar readings;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

// initialize outdoor readings
String outdoorTemp = "0";
String outdoorHumidity = "0";
unsigned long lastUpdate = 0;
unsigned long outdoorTimeout = 180000;  //disregard outdoor readings if older than limit set here
  
// Set static IP parameters
IPAddress local_IP(192, 168, 0, 194);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

// Create a sensor object
#define DHTPIN 13     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);

// Init AM2302
void initDHT(){ 
  dht.begin();
}

// Get Sensor Readings and return JSON object
String getSensorReadings(){
  readings["tempIndoor"] = String(dht.readTemperature(true));
  readings["humIndoor"] =  String(dht.readHumidity());
  readings["tempOutdoor"] = outdoorTemp;
  readings["humOutdoor"] = outdoorHumidity;
  String jsonString = JSON.stringify(readings);
  return jsonString;
}


// Initialize SPIFFS
void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

// Initialize WiFi
void initWiFi() {
// Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(3000);
  }
  Serial.println(WiFi.localIP());
}

void setup() { 
  Serial.begin(115200);  // Serial port for debugging purposes
  initDHT();
  initWiFi();
  initSPIFFS(); 
  Serial.println("inits done");

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.serveStatic("/", SPIFFS, "/");

  // Request for the latest sensor readings
  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = getSensorReadings();
    request->send(200, "application/json", json);
  });

  server.on("/update-sensors", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("received outdoor sensor update");
    AsyncWebParameter* p = request->getParam("temp");
    outdoorTemp = p->value();
    p = request->getParam("humidity");
    outdoorHumidity = p->value();
    lastUpdate = millis();
    request->send(200, "text/plain", "message received");
  });

  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);

  // Start server
  server.begin();
}



void loop() {
  if ((millis() - lastTime) > timerDelay) {
   // check wifi connection 
   if (WiFi.status() != WL_CONNECTED) {
    initWiFi();
   }
    
  // check to see if recent outdoor readings have been received - if not zero out values  
  if((lastUpdate + outdoorTimeout)  < millis())  {
    Serial.println("Outdoor sensors timed out");
    outdoorTemp = "0";
    outdoorHumidity = "0";
  }
    // Send Events to the client with the Sensor Readings Every timerDelay seconds
    events.send("ping",NULL,millis());
    events.send(getSensorReadings().c_str(),"new_readings" ,millis());
    lastTime = millis();
    Serial.println("data sent to event clients");
    Serial.println(getSensorReadings());
  }
}
