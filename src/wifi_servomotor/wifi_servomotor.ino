#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

/*
  Wemos d1 mini pin out

  Pin  Function                       ESP-8266 Pin
  TX  TXD                             TXD
  RX  RXD                             RXD
  A0  Analog input, max 3.3V input    A0
  D0  IO                              GPIO16
  D1  IO, SCL                         GPIO5
  D2  IO, SDA                         GPIO4
  D3  IO, 10k Pull-up                 GPIO0
  D4  IO, 10k Pull-up, BUILTIN_LED    GPIO2
  D5  IO, SCK                         GPIO14
  D6  IO, MISO                        GPIO12
  D7  IO, MOSI                        GPIO13
  D8  IO, 10k Pull-down, SS           GPIO15
  G   Ground                          GND
  5V  5V                              -
  3V3 3.3V                            3.3V
  RST Reset                           RST

  GPIO0: pull low during boot for flash mode (connects to a push button).
  Other than that, usable as GPIO but beware of waht you connect to it.
  GPIO1, 3: TX and RX (Serial). Also usable as GPIO if not using serial.
  GPIO2: is pulled up during boot & internal LED (active LOW).
  GPIO15: fixed external pull-down (for boot).
  GPIO4, 5, 12-14, 16: nothing special.
  Conclusion
  Stick to A0, D0, D5, D6 and D7.
*/
static const uint8_t D0 = 16;
static const uint8_t D1 = 5;
static const uint8_t D2 = 4;
static const uint8_t D3 = 0;
static const uint8_t D4 = 2;
static const uint8_t D5 = 14;
static const uint8_t D6 = 12;
static const uint8_t D7 = 13;
static const uint8_t D8 = 15;
static const uint8_t D9 = 3;
static const uint8_t D10 = 1;

Servo servo;
WiFiServer server(80);

// Only 2.4 GHz WiFi. ESP8266 does not support 5.0 GHz WiFi.
const char *ssid = "name";         //!!!!!!!!!!!!!!!!!!!!! modify this
const char *password = "password"; //!!!!!!!!!!!!!!!!!!!!! modify this

void setup()
{
    Serial.begin(9600);
    delay(10);

    // Connect to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
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
    servo.attach(D1);
    delay(1000);
}

void loop()
{
    // Check if a client has connected
    // and if there's no client, it goes to the deep sleep mode
    int sleep_timer = 5;
    WiFiClient client = server.available();
    while (!client)
    {
        Serial.println("No client connection was found");

        client = server.available();
        if (sleep_timer < 0)
        {
            Serial.println("Deep sleep for 5 seconds");
            ESP.deepSleep(5e6);
        }
        sleep_timer--;
        delay(1000);
        // return;
    }

    // Wait until the client sends some data
    while (!client.available())
    {
        delay(200);
    }
    Serial.println("new client");

    // Read the first line of the request
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    // Match the request
    int value = LOW;
    if (request.indexOf("/ON") != -1)
    {
        servo.write(140);
        value = HIGH;
    }

    if (request.indexOf("/OFF") != -1)
    {
        servo.write(40);
        value = LOW;
    }

    if (request.indexOf("/MID") != -1)
    {
        servo.write(90);
        value = LOW;
    }

    // Return the response
    client.println("HTTP/1.1 200 OK");
    client.println("Access-Control-Allow-Origin: *");
    client.println("Content-Type: text/html");
    client.println(""); //  do not forget this one
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");

    delay(500);
    client.stop();
    Serial.println("Client disconnected");
}
