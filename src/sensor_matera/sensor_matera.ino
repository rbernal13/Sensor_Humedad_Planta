#include <ESP8266WiFi.h>


/**
 * Datos del sensor de humedad YL-69
 **/
const int relay_pin = 0;
const int sensor_pin = A0;
const int rojo_pin = 16;
const int verde_pin = 5;
int sensor_data = 0;
int bomba = 0;
String arrayVariableNames[]={"Humedad","LedRojo","LedVerde","Bomba"};
int arrayVariableValues[]={10,0,0,0};

int numberVariables=sizeof(arrayVariableValues)/sizeof(arrayVariableValues[0]);



/**
 * Parametros para la red WIFI local y la URI Dweet.io
 **/
const char* ssid = "LaRed";
const char* password = "eslaclave";
const char* host= "dweet.io";
String sensor_name="donsensor_matera19"; // https://dweet.io/dweet/for/donsensor_matera19?estado


/**
 * Setup - Configuracion de WIFI Local
 **/
void setup() {
  
  Serial.begin(9600);

  //Iniciar LED's y Relay-Bomba
  pinMode(rojo_pin, OUTPUT);
  pinMode(verde_pin, OUTPUT);
  pinMode(relay_pin, OUTPUT);

  digitalWrite(relay_pin, HIGH);
  
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


/**
 * Loop - Main del sensor
 **/
void loop() {
	
	// Capturar datos
	sensor_data = analogRead(sensor_pin);
	int output_value = 0;
	output_value = map(sensor_data,1024,500,0,100);

  arrayVariableValues[0] = output_value;
  arrayVariableNames[0] = "Humedad";

  // Verificar data
  if (isnan(sensor_data)) {
    Serial.println("Error de lectura en el sensor YL-69");
    return;
  }
  
	Serial.println("Nivel de humedad: ");
	Serial.println(output_value);
	Serial.println(" %");

  int lrojo=-1, lverde=-1;

  //Verificar
  if(output_value <= 50 ){
    digitalWrite(rojo_pin, HIGH);
    digitalWrite(verde_pin, LOW);
    digitalWrite(relay_pin, LOW);
    delay(4000);
    digitalWrite(relay_pin, HIGH);
    
    lrojo=1;
    lverde=0;
    bomba=1;

    arrayVariableValues[1] = lrojo;
    arrayVariableNames[1] = "LedRojo";
    arrayVariableValues[2] = lverde;
    arrayVariableNames[2] = "LedVerde";
    arrayVariableValues[3] = bomba;
    arrayVariableNames[3] = "BombaON";
    
  }else{
    digitalWrite(verde_pin, HIGH);
    digitalWrite(rojo_pin, LOW);
    digitalWrite(relay_pin, HIGH);
    
    lrojo=0;
    lverde=1;
    bomba=0;

    arrayVariableValues[1] = lrojo;
    arrayVariableNames[1] = "LedRojo";
    arrayVariableValues[2] = lverde;
    arrayVariableNames[2] = "LedVerde";
    arrayVariableValues[3] = bomba;
    arrayVariableNames[3] = "BombaOFF";
  }
	
	// Enviar datos a dweet.io
  Serial.print("Enviando dweet a ");
  Serial.print(host);
  Serial.print("/follow/");
  Serial.print(sensor_name);
  Serial.println();
	
  // Creamos una URI para las peticiones
  // https://dweet.io/dweet/for/donsensor_matera19?estado
  enviarDweet(); //Conexion TCP y envio Dweet
  delay(2000);
}


/**
 * enviarDweet - Establecer conexion TCP para enviar el "dweet" a Dweet.io
 **/
void enviarDweet(){
	WiFiClient client;
	const int httpPort = 80;
  
  delay(5000);
	Serial.print("Esta conectandose a ");
	Serial.println(host);
  
  
	if (!client.connect(host, httpPort)){
		Serial.println("Conexion fallida a Dweet.io");
		return;
	}
  
	client.print(getDweetString());
	delay(10); //Esperar

  // Leemos todas las lineas desde la respuesta
	while (client.available()){
		String line = client.readStringUntil('\r');
		Serial.print(line);
	}
	Serial.println();
	Serial.println("Terminando conexion....");
}


/**
 * crearDweetString - Creacion del String con los datos para enviar a Dweet.io
 * Usar GET para postear en Dweet.io
 **/
String getDweetString(){
  int i = 0;
  
  // URI: https://dweet.io/dweet/for/donsensor_matera19?estado
  // sensor_name =  donsensor_matera19
  String dweetHttpGet="GET /dweet/for/";
  dweetHttpGet=dweetHttpGet+String(sensor_name)+"?";

  for(i=0;i<(numberVariables);i++){
    if(i==numberVariables-1){
      //the lastone doesnt have a "&" at the end
      dweetHttpGet=dweetHttpGet + String(arrayVariableNames[i]) + "=" + String(arrayVariableValues[i]);
    }
    else
      dweetHttpGet=dweetHttpGet + String(arrayVariableNames[i]) + "="+ String(arrayVariableValues[i]) + "&";
  }
  dweetHttpGet=dweetHttpGet+" HTTP/1.1\r\n"+
               "Host: " +
                 host +
               "\r\n" +
               "Conexion: terminada!!!\r\n\r\n";
  return dweetHttpGet;// Peticion HTTP con el String para enviar
}

/***
 * 
 * 
  String url = "/dweet/for/donsensor_matera19?estado";
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

***/
