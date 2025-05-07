#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TimeLib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// Datos del WiFi
const char* red_wifi = "Adri2025";
const char* clave_wifi = "1234567890";

// Pines para la pantalla TFT
#define TFT_CS   5
#define TFT_RST  4
#define TFT_DC   2
#define TFT_CLK  18
#define TFT_MOSI 23

Adafruit_ST7735 pantalla(TFT_CS, TFT_DC, TFT_RST);

// Cliente NTP para obtener la hora
WiFiUDP canalUDP;
NTPClient ntp(canalUDP, "pool.ntp.org", -5 * 3600, 60000);

// Colores
uint16_t fondo = ST77XX_WHITE;
uint16_t texto = ST77XX_BLACK;

void conectarWiFi() {
  WiFi.begin(red_wifi, clave_wifi);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado al WiFi");
}

void setup() {
  Serial.begin(115200);
  SPI.begin(TFT_CLK, -1, TFT_MOSI);

  pantalla.initR(INITR_BLACKTAB);
  pantalla.setRotation(1);
  pantalla.fillScreen(fondo);
  pantalla.setTextColor(texto);
  pantalla.setTextSize(2);

  conectarWiFi();

  ntp.begin();
  ntp.update();
  setTime(ntp.getEpochTime());
}

void loop() {
  static unsigned long anteriorMillis = 0;
  if (millis() - anteriorMillis > 60000) {
    ntp.update();
    setTime(ntp.getEpochTime());
    anteriorMillis = millis();
  }

  pantalla.fillScreen(fondo);
  pantalla.setCursor(10, 20);

  // Mostrar hora
  char hora[10];
  sprintf(hora, "%02d:%02d", hour(), minute());
  pantalla.print("Hora: ");
  pantalla.print(hora);

  // Mostrar fecha
  char fecha[15];
  sprintf(fecha, "%02d/%02d/%04d", day(), month(), year());
  pantalla.setCursor(10, 60);
  pantalla.print("Fecha: ");
  pantalla.print(fecha);

  delay(1000);
}
