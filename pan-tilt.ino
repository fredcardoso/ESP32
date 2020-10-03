#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Servo.h>
#include "pantilt_htm.h"

Servo servoX;
Servo servoY;

// GPIO the servo is attached to
static const int xPin = 13;
static const int yPin = 12;

// Replace with your network credentials
const char* ssid     = "rede";
const char* password = "senha";

WiFiServer server(80);

// Variable to store the HTTP request
String valueXY;

// Decode HTTP GET value
int pos1 = 0;
int pos2 = 0;
int xPos, yPos;

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  delay(15);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  
  servoX.attach(xPin);
  servoY.attach(yPin);
  servoX.write(90);
  delay(500);
  servoY.write(155);
  delay(500);
}

void loop(){
  WiFiClient client = server.available();

  if (client) {
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        valueXY += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Display the HTML web page
            client.println(pantilt_htm);
            
            // GET parameter values
            if(valueXY.indexOf("GET /?value=")>=0) {
              pos1 = valueXY.indexOf('=');
              pos2 = valueXY.indexOf(',');
              xPos = valueXY.substring(pos1+1, pos2).toInt();
              yPos = valueXY.substring(pos2+1, valueXY.length()).toInt();
                
            
              //Rotate the servo
              servoX.write(xPos);
              delay(800);
              servoY.write(yPos);
              delay(800);
            }
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    // Clear the valueXY variable
    valueXY = "";
  }
}
