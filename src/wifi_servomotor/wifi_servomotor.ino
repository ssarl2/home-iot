#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

Servo servo;
WiFiServer server(80);

int motorPin = D1;

// Only 2.4 GHz WiFi. ESP8266 does not support 5.0 GHz WiFi.
const char* ssid = "name";             //!!!!!!!!!!!!!!!!!!!!! modify this
const char* password = "password";     //!!!!!!!!!!!!!!!!!!!!! modify this

void setup() {

    Serial.begin(9600);
    delay(10);

    // Connect to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");

    // Start the server
    server.begin();
    Serial.println("Server started");

    // Print the IP address
    Serial.print("Use this URL : ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");

    // Connect servomotor
    servo.attach(5); //D1
    servo.write(90);
    delay(1000);
}

void loop() {

    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client) {
        return;
    }

    // Wait until the client sends some data
    Serial.println("new client");
    while(!client.available()){
        delay(10);
    }

    // Read the first line of the request
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    // Match the request

    int value = LOW;
    if (request.indexOf("/ON") != -1) {
        servo.write(140);
        value = HIGH;
    } 
    if (request.indexOf("/OFF") != -1){
        servo.write(40);
        value = LOW;
    }
    if (request.indexOf("/MID") != -1){
        servo.write(90);
        value = LOW;
    }

    // Return the response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); //  do not forget this one
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");

    client.print("Servo motor is turned now: ");

    if(value == HIGH) {
        client.print("On");  
    } else {
        client.print("Off");
    }
    client.println("<br><br>");
    client.println("Click <a href=\"/ON\">ON</a> turn the Motor<br>");
    client.println("<br><br>");
    client.println("Click <a href=\"/OFF\">OFF</a> turn the Motor<br>");
    client.println("<br><br>");
    client.println("Click <a href=\"/MID\">MID</a> turn the Motor<br>");
    client.println("</html>");

    delay(1);
    Serial.println("Client disconnected");
    Serial.println("");

}
