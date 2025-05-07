#include <WiFi.h>
#include <ThingerESP32.h>

#define USUARIO "adri_lux2"
#define DISPOSITIVO "LEDRGB"
#define TOKEN "24681012"

ThingerESP32 miDispositivo(USUARIO, DISPOSITIVO, TOKEN);

const char* wifiNombre = "Adri2025";
const char* wifiClave = "1234567890";

int ledNormal = 2;
int ledRojo = 27;
int ledVerde = 26;
int ledAzul = 25;

void setup() {
  miDispositivo.add_wifi(wifiNombre, wifiClave);

  pinMode(ledNormal, OUTPUT);

  miDispositivo["Encender_LED"] << [](pson& valor){
    if (valor.is_empty()) {
      valor = digitalRead(ledNormal);
    } else {
      digitalWrite(ledNormal, valor ? HIGH : LOW);
    }
  };

  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAzul, OUTPUT);

  miDispositivo["Colores_RGB"] << [](pson& color){
    analogWrite(ledRojo, color["R"]);
    analogWrite(ledVerde, color["G"]);
    analogWrite(ledAzul, color["B"]);
  };
}

void loop() {
  miDispositivo.handle();
}
