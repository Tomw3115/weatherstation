/*********
  Rui Santos
  Complete instructions at https://RandomNerdTutorials.com/esp32-web-server-gauges/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  This is a new module to support slave sensors on a separate ESP32 device.  It uses the DHT11 sensor
  to read the humidity and temperature (F) and then transmit a GET message string to the server on a periodic basis.
  It was written by Tom Wilson, Brasilito, Costa Rica.  Permission to use is granted under the same conditions as the original code.
*********/

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

// Wifi login - replace with your network credentials
const char* ssid = "MY SSID";
const char* password = "MY PASSWORD";

// wifi address of master device listener
String serverName = "http://192.168.0.194/update-sensors";

// Timer variables - used to display readings on the serial port
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

// Set static IP parameters
IPAddress local_IP(192, 168, 0, 195);
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

// Get Sensor Readings and return data in a GET request string format
String getSensorReadings(){
  String returnString = "?temp=" + String(dht.readTemperature(true));
  returnString += "&humidity=" + String(dht.readHumidity());
  return returnString;
}

// Initialize WiFi
void initWiFi() {
// Configures static IP address
  WiFi.setHostname("WeatherRemote");
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}
String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP GET request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
void setup() {
  Serial.begin(115200);  // Serial port for debugging purposes
  initDHT();
  initWiFi();
  Serial.println("setup done");
}

void loop() {
  // loop to send readings to indoor unit and print data on serial port for diagnostics
  if ((millis() - lastTime) > timerDelay) {
      lastTime = millis();
      //Serial.println(getSensorReadings());
      if (WiFi.status() != WL_CONNECTED){initWiFi();}

    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
    
      // set GET request string
      String requestString = serverName +  getSensorReadings();
      String response;
      Serial.println(requestString);
      response = httpGETRequest(requestString.c_str());
      Serial.println(response);
      
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }    
  
}
