#include "DHT.h"

// Pin
#define DHTPIN D1

// Use DHT11 sensor
#define DHTTYPE DHT11

// Initialize DHT sensor
//DHT dht(DHTPIN, DHTTYPE, 15);

//LED on ESP8266 GPIO2
const int lightPin = 2;


//WIFI Datos y Dweet.io
const char* ssid = "Movistar_15683961";
const char* password = "0096769669";
const char* host= "dweet.io";
String sensor="ElSensor"
const int httpPort = 80;
WiFiClient client;


void setup() {
  // Iniciar sensor:
  pinMode(lightPin, OUTPUT);
  digitalWrite(lightPin, LOW);
  
  Serial.begin(9600); 
  //dht.begin();
  //delay(15);
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
  delay(5000);
  Serial.print("Esta conectado a ");
  Serial.println(host);

  // Crear conexion TCP
  if (!client.connect(host, httpPort)) {
    Serial.println("Conexion fallida");
    return;
  }

  // Obtenemos la lectura del led
  float led = analogRead(lightPin);

  Serial.print("Estado del Led: ");
  Serial.println(led);
   
  // Verificamos si existe alguna lectura fallida
  if (isnan(led)) {
    Serial.println("Error de lectura en el sensor DHT");
    return;
  }

  // Creamos una URI para las peticiones
  String url = "/dweet/for/lamatera_sensor19?estado=";
  url += String(led);

  // Envio la peticion
  Serial.print("URL de solicitud: ");
  Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Conexion: terminada!!!\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Tiempo de espera agotado!!!");
      client.stop();
      return;
    }
  }

  // Leemos todas las lineas desde la respuesta
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  // Cerramos la conexion
  Serial.println();
  Serial.println("Cerrando conexion");
}
