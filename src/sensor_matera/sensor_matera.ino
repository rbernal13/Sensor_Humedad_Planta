#include <ESP8266WiFi.h>

// Sensor
const int sensor_pin = A0;
int sensor_data = 0;


//WIFI Datos y Dweet.io
const char* ssid = "LaRed";
const char* password = "eslaclave";
const char* host= "dweet.io";
String sensor="DonSensor";


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
  
  //Capturar datos
  sensor_data = analogRead(sensor_pin);
  Serial.println(sensor_data);

  enviarDweet();

  
  
  
  // put your main code here, to run repeatedly:
  delay(5000);
  Serial.print("Esta conectado a ");
  Serial.println(host);

  // Crear conexion TCP
  if (!client.connect(host, httpPort)) {
    Serial.println("Conexion fallida");
    return;
  }

  
  //output_value = map(output_value,1024,500,0,100);
  

  if (digitalRead (led) == HIGH){
    Serial.print("Estado del Led: ");
    Serial.println(led);
    Serial.print("Encendido!");
  } else{
    Serial.print("Estado del Led: ");
    Serial.print("Apagado!");
  }
  
   
  // Verificamos si existe alguna lectura fallida
  
  /*if (isnan(led)) {
    Serial.println("Error de lectura en el sensor DHT");
    return;
  }
  */
  
  // Creamos una URI para las peticiones
  // https://dweet.io/dweet/for/lamatera_sensor?estado
  
  String url = "/dweet/for/lamatera_sensor19?estado=";
  url += String(DHTPIN);

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

void enviarDweet(){
  WiFiClient client;
  const int httpPort = 80;
  
  if (!client.connect(host, httpPort)){
    Serial.println("Conexion fallida a Dweet.io");
    return;
  }
  
  client.print(getDweetString());
  delay(10); //wait...
  while (client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");
  
		
}
