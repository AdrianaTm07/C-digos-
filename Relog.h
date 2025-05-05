#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TimeLib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

const char* red_wifi = "Adri2025";
const char* clave_wifi = "1234567890";

const uint8_t PIN_CS    = 5;
const uint8_t PIN_RST   = 4;
const uint8_t PIN_DC    = 2;
const uint8_t PIN_CLK   = 18;
const uint8_t PIN_MOSI  = 23;

Adafruit_ST7735 display(PIN_CS, PIN_DC, PIN_RST);

WiFiUDP canalUDP;
NTPClient sincronizador(canalUDP, "pool.ntp.org", -5 * 3600, 60000);

// Colores personalizados
uint16_t color_texto = ST77XX_BLACK;
uint16_t color_fondo = display.color565(255, 182, 193);  // Rosa claro (Light Pink)

const int digitos[10][7][4] = {
  {{0,0,12,60},{12,0,16,4},{28,0,4,60},{12,56,16,4}}, // 0
  {{10,0,16,4},{16,4,10,52},{10,56,18,4}},           // 1
  {{0,0,32,4},{0,4,4,14},{24,4,8,26},{0,30,32,4},{0,34,12,22},{28,48,4,12},{0,56,32,4}}, // 2
  {{0,0,32,4},{0,4,4,14},{24,4,8,52},{8,30,20,4},{0,48,4,12},{0,56,32,4}}, // 3
  {{0,0,4,30},{0,30,24,4},{24,0,8,60}},               // 4
  {{0,0,32,4},{0,4,12,26},{28,4,4,12},{0,30,32,4},{24,34,8,26},{0,48,4,12},{0,56,32,4}}, // 5
  {{0,0,32,4},{0,4,12,52},{28,4,4,12},{12,30,20,4},{28,34,4,26},{0,56,32,4}}, // 6
  {{0,0,32,4},{0,4,4,14},{24,4,8,56}},               // 7
  {{0,0,12,60},{12,0,20,4},{12,30,20,4},{32,0,4,60},{12,56,20,4}}, // 8
  {{0,0,32,4},{0,4,4,26},{24,4,8,52},{0,30,24,4},{0,48,4,12},{0,56,32,4}}  // 9
};

void mostrarDigito(uint8_t valor, int x, int y, bool limpiar = true) {
  if (limpiar) display.fillRect(x, y, 36, 60, color_fondo);
  for (int i = 0; i < 7; i++) {
    int ancho = digitos[valor][i][2];
    int alto = digitos[valor][i][3];
    if (ancho == 0 || alto == 0) continue;
    display.fillRect(x + digitos[valor][i][0], y + digitos[valor][i][1], ancho, alto, color_texto);
  }
}

void mostrarSeparador(int x, int y) {
  display.fillRect(x, y + 18, 5, 5, color_texto);
  display.fillRect(x, y + 40, 5, 5, color_texto);
}

void renderizarHora() {
  int h = hour();
  int m = minute();

  int h1 = h / 10, h2 = h % 10;
  int m1 = m / 10, m2 = m % 10;

  display.fillRect(0, 0, 160, 80, color_fondo);

  mostrarDigito(h1, 6, 10);
  mostrarDigito(h2, 42, 10);
  mostrarSeparador(78, 10);
  mostrarDigito(m1, 90, 10);
  mostrarDigito(m2, 126, 10);
}

void renderizarFecha() {
  display.fillRect(0, 85, 160, 45, color_fondo);

  char cadena_fecha[20];
  sprintf(cadena_fecha, "%02d-%02d-%02d", day(), month(), year() % 100);

  const char* nombres_dia[] = {"DOMINGO", "LUNES", "MARTES", "MIÉRCOLES", "JUEVES", "VIERNES", "SÁBADO"};
  const char* dia_actual = nombres_dia[weekday() - 1];

  display.setTextColor(color_texto);
  display.setTextSize(1);
  display.setCursor(5, 95);
  display.print(cadena_fecha);
  display.setCursor(70, 95);
  display.print(dia_actual);
}

void setup() {
  Serial.begin(115200);
  SPI.begin(PIN_CLK, -1, PIN_MOSI);
  display.initR(INITR_BLACKTAB);
  display.setRotation(1);
  display.fillScreen(color_fondo);

  WiFi.begin(red_wifi, clave_wifi);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("*");
  }

  sincronizador.begin();
  sincronizador.update();
  setTime(sincronizador.getEpochTime());
}

unsigned long ultima_actualizacion = 0;
int hora_previa = -1;
int minuto_previo = -1;

void loop() {
  if (millis() - ultima_actualizacion > 60000) {
    sincronizador.update();
    setTime(sincronizador.getEpochTime());
    ultima_actualizacion = millis();
  }

  int hora_actual = hour();
  int minuto_actual = minute();

  if (hora_actual != hora_previa || minuto_actual != minuto_previo) {
    renderizarHora();
    renderizarFecha();
    hora_previa = hora_actual;
    minuto_previo = minuto_actual;
  }

  delay(200);
}
