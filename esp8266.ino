/*
   Este sketch es para controlar 4 salidas digitales distintas
   tambien tiene un dht11 para enviar la temperatura y un bluetooth para conectarse de manera local.

  Circuito:
  NodeMCU v1.0
  DHT11
  Modulo Blutooth hc-06 o hc-05
  4 modulos de relay o relays
  led amarillo, rojo.
  resistencia 10k para el dht11


   Yeffri J. Salazar
   Hackerspace Xibalba y Comunidad Arduino Guatemala
   Themicrofcontrol.wordpress.com

*/
/************************************
 **              Librerias         **
 ***********************************/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
/************************************
 **              constantes         **
 ***********************************/
#define salida1 D5
#define salida2 D6
#define salida3 D7
#define salida4 D8
#define ledAmarillo D4
#define ledRojo D1
#define pinDHT 9
#define puertoMqtt 1883
/***********************************
 **       objetos y variables      **
 ***********************************/
DHT dht;
WiFiClient clienteWifi;//este cliente se encarga de la comunicacion con el wifi
PubSubClient clienteMQTT(clienteWifi);//este utiliza el cliente anterior para hacer poder crear la conexion mqtt
//si pasan por el hackerspace Xibalba pues ya tienen la clave
const char * ssid = "Hackerspace";
const char * claveWifi = "IOT12345";
const char * brokerMqtt = "192.168.1.10";// ip del broker sin http ni nada solo los numeros
uint32_t ultimoIntentoReconexion;
uint32_t timerEnvioDatos;
uint32_t cambioEstado = 0;
uint8_t estadoLed = 0;
uint16_t tiempoParpadeo = 350;
float humedad, temperatura;


void parpadeo(uint8_t led) {
  //Serial.println(millis() - cambioEstado);
  if ( millis() - cambioEstado > tiempoParpadeo) {
    //     Serial.println(pasoSemaforo);
    cambioEstado = millis();
    estadoLed = !estadoLed;
  }
  digitalWrite(led, estadoLed);
}


void conectarAlWifi() {

  WiFi.begin(ssid, claveWifi);
  Serial.print("conectando a");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wifi Conectado ");
  Serial.println("direccion IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* mensaje, unsigned int length) {
  String topico = topic;
  Serial.print("Mensaje Recibido del topico: ");
  Serial.println(topico);
  Serial.print("mensaje : ");
  for (uint8_t i = 0; i < length; i++) {
    Serial.print(mensaje[i]);
  }
  //le restamos -48 para que el valor sea 0 o 1
  //restamos el valor ascii para hacerlo un entero

  if (topico == "/salida1") {
    digitalWrite(salida1, mensaje[0] - 48);
    Serial.println("Salida 1 ");
  }
  else if (topico == "/salida2") {
    digitalWrite(salida2, mensaje[0] - 48);
    Serial.println("Salida 2 ");
  }
  else if (topico == "/salida3") {
    digitalWrite(salida3, mensaje[0] - 48);
    Serial.println("Salida 3 ");
  }
  else if (topico == "/salida4") {
    digitalWrite(salida4, mensaje[0] - 48);
    Serial.println("Salida 4 ");
  }
  else if (topico == "/temperatura") {
    Serial.println(temperatura);
    char msg[3];
    snprintf (msg, 3, "%ld", (int)temperatura);
    clienteMQTT.publish("/respuestaTemperatura", msg);
  }
  else if (topico == "/humedad") {
    Serial.println(humedad);
    char msg[3];
    snprintf (msg, 3, "%ld", (int)temperatura);
    clienteMQTT.publish("/respuestaTemperatura", msg);
  }
  else {
    Serial.println("error de mensaje");
  }

}

boolean reconexion() {
  Serial.print("Conectando al broker mqtt");
  //intentando conectar al broker
  if (clienteMQTT.connect("ESP8266Client")) {
    Serial.println("Conectado");
    //publicamos que estamos conectados
    clienteMQTT.publish("/conexion", "Conectado");
    //nos suscribimos a los topicos para controlar los ledes
    clienteMQTT.subscribe("/salida1");
    clienteMQTT.subscribe("/salida2");
    clienteMQTT.subscribe("/salida3");
    clienteMQTT.subscribe("/salida4");
    clienteMQTT.subscribe("/temperatura");
    clienteMQTT.subscribe("/humedad");
  } else {
    Serial.print("falló, rc=");
    Serial.print(clienteMQTT.state());
  }
  return clienteMQTT.connected();
}

void entradaSerial() {
  if (Serial.available()) {
    char dato = Serial.read();
    if (dato == 'a') {
      digitalWrite(salida1, HIGH);
      Serial.println("Salida 1 ");
    }
    else if (dato == 's') {
      digitalWrite(salida2, HIGH);
      Serial.println("Salida 2 ");
    }
    else if (dato == 'd') {
      digitalWrite(salida3, HIGH);
      Serial.println("Salida 3 ");
    }
    else if (dato == 'f') {
      digitalWrite(salida4, HIGH);
      Serial.println("Salida 4 ");
    }
    else if (dato == 'q') {
      digitalWrite(salida1, LOW);
      Serial.println("Salida 1 ");
    }
    else if (dato == 'w') {
      digitalWrite(salida2, LOW);
      Serial.println("Salida 2 ");
    }
    else if (dato == 'e') {
      digitalWrite(salida3, LOW);
      Serial.println("Salida 3 ");
    }
    else if (dato == 'r') {
      digitalWrite(salida4, LOW);
      Serial.println("Salida 4 ");
    }
    else if (dato == ' ') {
      digitalWrite(salida4, LOW);
      digitalWrite(salida3, LOW);
      digitalWrite(salida2, LOW);
      digitalWrite(salida1, LOW);
      Serial.println("apagando todo");
    }
    else if (dato == '1') {
      digitalWrite(salida4, HIGH);
      digitalWrite(salida3, HIGH);
      digitalWrite(salida2, HIGH);
      digitalWrite(salida1, HIGH);
      Serial.println("encendiendo todo");
    }

    else {
      Serial.println("error de mensaje");
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("iniciando programa Holi mami");
  dht.setup(pinDHT); // dht PIN
  pinMode(salida1, OUTPUT);
  pinMode(salida2, OUTPUT);
  pinMode(salida3, OUTPUT);
  pinMode(salida4, OUTPUT);
  pinMode(ledAmarillo, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  digitalWrite(ledAmarillo, HIGH);
  digitalWrite(ledRojo, HIGH);
  conectarAlWifi();
  clienteMQTT.setServer(brokerMqtt, puertoMqtt); //le decimos cual es el servidor y el puerto al que se debe conectar
  clienteMQTT.setCallback(callback);//le decimos como se llama la funcion de callback
}

void loop() {
  entradaSerial();
  if (!clienteMQTT.connected()) {
    if (millis() - ultimoIntentoReconexion > 5000) {
      ultimoIntentoReconexion = millis();
      // Attempt to reconnect
      if (reconexion()) {
        ultimoIntentoReconexion = 0;
      }
    }
  } else {
    //cliente conectado
    if (millis() - timerEnvioDatos > 5000) {
      timerEnvioDatos = millis();
      //a falta de sensores enviamos valores aleatorios
      char msg[3];
      humedad = dht.getHumidity();
      temperatura = dht.getTemperature();
      snprintf (msg, 3, "%ld", (int)temperatura);
      clienteMQTT.publish("/temperatura", msg);
      snprintf (msg, 3, "%ld", (int)humedad);
      clienteMQTT.publish("/humedad", msg);
      Serial.print(dht.getStatusString());
      Serial.print("\t");
      Serial.print(humedad);
      Serial.print("\t\t");
      Serial.print(temperatura);
    }
    clienteMQTT.loop();
    parpadeo(D1);
    parpadeo(D4);
  }
}
