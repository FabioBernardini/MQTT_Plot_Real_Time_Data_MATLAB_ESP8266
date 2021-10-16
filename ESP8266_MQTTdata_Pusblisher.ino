/*
 * Author = Fabio Bernardini
 * 
 * In this script, the ESP8266 recevies the data from a DHT11 sensor.
 * Then, it sends them through a MQTT communication
 * 
 */

#include <Adafruit_Sensor.h> //Library for Adafruit sensors , we are using for DHT
#include <DHT.h> //DHT library which uses some func from Adafruit Sensor library
#include <DHT_U.h>
#include <ESP8266WiFi.h> //library for using ESP8266 WiFi 
#include <PubSubClient.h> //library for MQTT
#include <ArduinoJson.h> //library for Parsing JSON
 
//defining Pins
#define DHTPIN 5
const int pinLED = 2;
 
//DHT parameters
#define DHTTYPE DHT11     // DHT 11
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;
 
//MQTT Credentials
const char* ssid = "NETGEAR07";//setting your ap ssid
const char* password = "magicalsheep499";//setting your ap psk
const char* mqttServer = "broker.emqx.io"; //MQTT URL
const char* clientID = "ESP8266"; // client id username+0001
const char* topic = "ESP8266/dataSensor"; //publish topic
 
//parameters for using non-blocking delay
unsigned long previousMillis = 0;
const long interval = 5000;
 
String msgStr = "";      // MQTT message buffer
 
float temp, hum;
 
 
//setting up wifi and mqtt client
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup_wifi() {
  delay(10);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
}
 
void reconnect() {
  while (!client.connected()) {
    if (client.connect(clientID)) {
      Serial.println("MQTT connected");
      client.subscribe("Led/status");
      Serial.println("Topic Subscribed");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);  // wait 5sec and retry
    }
 
  }
 
}
 
//subscribe call back
void callback(char*topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  String data = "";
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    data += (char)payload[i];
  }
 
 
 
  if (data == "1") {
    Serial.println("LED");
    digitalWrite(pinLED, LOW);
  }
  else {
    digitalWrite(pinLED, HIGH);
  }
}
 
 
void setup() {
  Serial.begin(57600);
  // Initialize device.
  dht.begin();
  // get temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
 
  pinMode(pinLED, OUTPUT);
  digitalWrite(pinLED, LOW);
 
  setup_wifi();
 
  client.setServer(mqttServer, 1883); //setting MQTT server
  client.setCallback(callback); //defining function which will be called when message is recieved.
  randomSeed(analogRead(0));
 
 
}
 
void loop() {
  if (!client.connected()) { //if client is not connected
    reconnect(); //try to reconnect
  }
  client.loop();
 
  unsigned long currentMillis = millis(); //read current time
 
  if (currentMillis - previousMillis >= interval) { //if current time - last time > 5 sec
    previousMillis = currentMillis;
 
    //read temp and humidity
    sensors_event_t event;
    dht.temperature().getEvent(&event);
 
 
    if (isnan(event.temperature)) {
      Serial.println(F("Error reading temperature!"));
    }
    else {
      Serial.print(F("Temperature: "));
      temp = event.temperature;
      Serial.print(temp);
      Serial.println(F("°C"));
    }
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      Serial.println(F("Error reading humidity!"));
    }
    else {
      Serial.print(F("Humidity: "));
      hum = event.relative_humidity;
      Serial.print(hum);
      Serial.println(F("%"));
    }
 
    msgStr = "{\"parameters\" : { \"temp\" : " + String(temp) + ",\"humidity\": " + String(hum) + "}}";
    byte arrSize = msgStr.length() + 1;
    char msg[arrSize];
 
    Serial.print("PUBLISH DATA:");
    Serial.println(msgStr);
    msgStr.toCharArray(msg, arrSize);
    client.publish(topic, msg);
    msgStr = "";
    delay(50);
 
  }
 
}
