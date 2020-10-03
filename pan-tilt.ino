#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Servo.h>
#include "pantilt_htm.h"

Servo servoX;
Servo servoY;

// Replace with your network credentials
const char* ssid     = "rede";
const char* password = "senha";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String valueXY;


// Decode HTTP GET value
int pos1 = 0;
int pos2 = 0;
int xPos, yPos;


// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  delay(10);

  servoX.attach(13);  // attaches the servo on the servoPin to the servo object
  servoY.attach(12);
  servoX.write(90);
  delay(15);
  servoY.write(155);
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
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        valueXY += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP valueXYs always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Display the HTML web page
            client.println(pantilt_htm);
            
            //GET /?value=180& HTTP/1.1
            if(valueXY.indexOf("GET /?value=")>=0) {
              pos1 = valueXY.indexOf('=');
              pos2 = valueXY.indexOf(',');
              xPos = valueXY.substring(pos1+1, pos2).toInt();
              yPos = valueXY.substring(pos2+1, valueXY.length()).toInt();
                
            
              //Rotate the servo
              servoX.write(xPos);
              delay(100);
              servoY.write(yPos);
              delay(100);
            }
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the valueXY variable
    valueXY = "";
  }
}
