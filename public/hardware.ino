

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>
#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000
};

const char* ssid = "real"; //Enter SSID
const char* password = ""; //Enter Password
const char* websockets_server_host = "signrecognitionsystem.herokuapp.com"; //Enter server adress
const uint16_t websockets_server_port = 80; // Enter server port
using namespace websockets;
WebsocketsClient client;

void setup() {
  Serial.begin(115200); 
  

  // Connect to wifi
  WiFi.begin(ssid, password);
  // Wait some time to connect to wifi
  for (int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
    Serial.print(".");
    delay(1000);
  }
  // Check if connected to wifi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("No Wifi!");
    return;
  }
  Serial.println("Connected to Wifi, Connecting to server.");
  // try to connect to Websockets server
  bool connected = client.connect(websockets_server_host, websockets_server_port, "/");
  if (connected) {
    Serial.println("Connected!");
  } else {
    Serial.println("Not Connected!");
  }
  // run callback when messages are received
  client.onMessage([&](WebsocketsMessage message) {
    delay(500);
    Serial.print("Got Message: ");
    Serial.println(message.data());
  });


  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }



}

void loop() {

  // let the websockets client check for incoming messages
  client.poll();

  // Let all the clients know that you are alive !
  client.onMessage([&](WebsocketsMessage message) {
    Serial.println("Got Message: ");
    Serial.println(message.data());
    //Deserialise the json !
    StaticJsonDocument<256> doc;
    DeserializationError err = deserializeJson(doc, message.data());
    if (err) {
      Serial.print("Error: ");
      Serial.println(err.c_str());
      return;
    }
    String gestureStatus = doc["gestureName"];

    if (gestureStatus == "One") {
      display.clearDisplay();

      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(50, 28);
      display.println("One");
      display.display();
//      delay(2000);
      display.clearDisplay();
    } else if (gestureStatus == "Class 2") {
      display.clearDisplay();

      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(50, 28);
      display.println("Two");
      display.display();
//      delay(2000);
      display.clearDisplay();
    } else if (gestureStatus == "Normal") {
      display.clearDisplay();

      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(50, 28);
      display.println("Normal");
      display.display();
//      delay(2000);
      display.clearDisplay();
    } else {
      display.clearDisplay();

      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(50, 28);
      display.println("No signs");
      display.display();
//      delay(2000);
      display.clearDisplay();
    }

  });


    display.clearDisplay();

      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(10, 20);
      display.println("Sign Recognition");
      display.display();
//      delay(2000);
      display.clearDisplay();


}