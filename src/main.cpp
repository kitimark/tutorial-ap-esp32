/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <Arduino.h>
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

// Replace with your network credentials
const char* ssid     = "ESP32-Access-Point";
const char* password = NULL;

// Set web server port number to 80
AsyncWebServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

// Assign output variables to GPIO pins
const int output26 = 26;
const int output27 = 27;

void setup() {
  Serial.begin(9600);

  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Initialize the output variables as outputs
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncResponseStream *response = request->beginResponseStream("text/html");

    if (request->hasParam("26")) {
      AsyncWebParameter *p = request->getParam("26");
      if (p->value() == "on") {
        digitalWrite(26, HIGH);
        output26State = "on";
      }
      if (p->value() == "off") {
        digitalWrite(26, LOW);
        output26State = "off";
      }
    }

    if (request->hasParam("27")) {
      AsyncWebParameter *p = request->getParam("27");
      if (p->value() == "on") {
        digitalWrite(27, HIGH);
        output27State = "on";
      }
      if (p->value() == "off") {
        digitalWrite(27, LOW);
        output27State = "off";
      }
    }

    response->println("<!DOCTYPE html><html>");
    response->println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    response->println("<link rel=\"icon\" href=\"data:,\">");
    // CSS to style the on/off buttons 
    // Feel free to change the background-color and font-size attributes to fit your preferences
    response->println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
    response->println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
    response->println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
    response->println(".button2 {background-color: #555555;}</style></head>");
    
    // Web Page Heading
    response->println("<body><h1>ESP32 Web Server</h1>");
    response->println("<p>RED 26 - State " + output26State + "</p>");
    // If the output26State is off, it displays the ON button       
    if (output26State=="off") {
      response->println("<p><a href=\"/?26=on\"><button class=\"button button2\">ON</button></a></p>");
    } else {
      response->println("<p><a href=\"/?26=off\"><button class=\"button\">OFF</button></a></p>");
    } 
    response->println("<p>RED 27 - State " + output27State + "</p>");
    if (output27State=="off") {
      response->println("<p><a href=\"/?27=on\"><button class=\"button button2\">ON</button></a></p>");
    } else {
      response->println("<p><a href=\"/?27=off\"><button class=\"button\">OFF</button></a></p>");
    } 
    response->println("</body></html>");
    request->send(response);
  });

  server.on("/chat", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("text")) {
      Serial.print(request->client()->remoteIP());
      Serial.print("\t");
      AsyncWebParameter *p = request->getParam("text");
      Serial.println(p->value());
    }
    request->send(SPIFFS, "/chat.html", String(), false);
  });
  
  server.begin();
}

void loop(){
}