#include <WiFi.h>
#include <ThingerESP32.h>

#define USER_NAME "adri_lux2"
#define DEVICE_ID "LEDRGB"
#define DEVICE_CREDENTIAL "24681012"

ThingerESP32 node(USER_NAME, DEVICE_ID, DEVICE_CREDENTIAL);

const char* SSID = "Adri2025";
const char* PASSWORD = "1234567890";

const int PIN_LED_SIMPLE = 2;
const int PIN_RGB_RED = 27;
const int PIN_RGB_GREEN = 26;
const int PIN_RGB_BLUE = 25;

void setup() {
  node.add_wifi(SSID, PASSWORD);

  pinMode(PIN_LED_SIMPLE, OUTPUT);

  node["Led_Simple"] << [](pson& data){
    if(data.is_empty()){
      data = (bool) digitalRead(PIN_LED_SIMPLE);
    } else {
      digitalWrite(PIN_LED_SIMPLE, data ? HIGH : LOW);
    }
  };

  pinMode(PIN_RGB_RED, OUTPUT);
  pinMode(PIN_RGB_GREEN, OUTPUT);
  pinMode(PIN_RGB_BLUE, OUTPUT);

  node["Led_RGB"] << [](pson& color){
    analogWrite(PIN_RGB_RED, color["R"]);
    analogWrite(PIN_RGB_GREEN, color["G"]);
    analogWrite(PIN_RGB_BLUE, color["B"]);
  };
}

void loop() {
  node.handle();
}
