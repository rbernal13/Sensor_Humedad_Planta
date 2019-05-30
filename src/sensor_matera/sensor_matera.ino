#include "DHT.h"

// Pin
#define DHTPIN D1

// Use DHT11 sensor
#define DHTTYPE DHT11

// Initialize DHT sensor
//DHT dht(DHTPIN, DHTTYPE, 15);

//WIFI Datos y Dweet.io
const char* ssid = "Movistar_15683961";
const char* password = "0096769669";
const char* host= "dweet.io";
String sensor="ElSensor"


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  // Iniciar conexion WIFI
  Serial.println();
  Serial.println();
  Serial.print("Conectandose a  "); 
  Serial.println(ssid); 
  // Conectarse a red WIFI LAN 
  WiFi.begin(ssid, password);   
  int intentos = 0; 
  while ((WiFi.status() != WL_CONNECTED) && (intentos < 15)) {
    intentos++; 
    delay(500); 
    Serial.print(".");
    } 
  if(intentos>14){
    Serial.println(F("Conexion WIFI fallida!"));
    }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(F("WIFI Conectado!"));
    Serial.println("Tu direccion IP: ");
    Serial.println(WiFi.localIP());
    Serial.println(F("======================================================"));
    } 
  Serial.println(F("Config lista"));
}

void loop() {
  // put your main code here, to run repeatedly:

}
