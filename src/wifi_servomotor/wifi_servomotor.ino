#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

Servo servo;
WiFiServer server(80);

int motorPin = D1;

const char* ssid = "abc";                           //!!!!!!!!!!!!!!!!!!!!! modify this
const char* password = "1234abcd!!";                //!!!!!!!!!!!!!!!!!!!!!modify this

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
    servo.write(0);
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
    if (request.indexOf("/Motor=ON") != -1) {
        servo.write(180);
        // digitalWrite(motorPin, HIGH);
        value = HIGH;
    } 
    if (request.indexOf("/Motor=OFF") != -1){
        servo.write(0);
        // digitalWrite(motorPin, LOW);
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
    client.println("Click <a href=\"/Motor=ON\">here</a> turn the Motor<br>");
    client.println("Click <a href=\"/Motor=OFF\">here</a> turn the Motor<br>");
    client.println("</html>");

    delay(1);
    Serial.println("Client disconnected");
    Serial.println("");

}