#include <ESP8266WiFi.h>

//WIFI Datos y Dweet.io
const char* ssid = "LaRed";
const char* password = "qwerty123";
const char* host= "dweet.io";
String sensor="ElSensor"

//Variables Sensor
String arrayVariableNames[]={"humedad"}; // Nombre de nuestro(s) sensores(s)
int arrayVariableValues[]={"25"} // Valor de nuestro sensor o sensores
int numberVariables=sizeof(arrayVariableValues)/sizeof(arrayVariableValues[0]); // tamaño del argumento para cada variable


void setup() { 
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

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 50, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("topic", msg);
  }
}
