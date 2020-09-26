
//  _____   _____  _______ __   _ _______  _____         _______  ______   _____  _____ 
// |     | |_____] |______ | \  | |______ |     | |      |_____| |_____/     |   |     |
// |_____| |       |______ |  \_| ______| |_____| |_____ |     | |    \_ . __|__ |_____|

// Copyright (C) 2020 opensolar
// Author: Martin Lafleur (mlafleur@opensolar.io)
// Date: Sep 2020
// Project: OpenOne
// Description: ESP8266 code for OpenOne

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>

const int GPIO_SS = 15; 
const int NB_HALLS = 4;
enum motor_modes { BRAKE = 0, CW = 1, CCW = 2, COAST = 3};

typedef struct{
  uint8_t mode; // see motor_modes enum
  uint8_t spare;
  uint16_t speed;   // 65535 = 99.9%
  
} MOTOR;

typedef struct{
  uint8_t digit[2];
  uint8_t bits;
  uint8_t spare;
} CONTROL;
enum control_bits { SPARE1 = 0x02, STANDBY = 0x01 };



const char* ssid = "laflaulac";
const char* password = "9876543210";
const char* mqtt_server = "192.168.0.200";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("opensolar", "hello world");
      // ... and resubscribe
      client.subscribe("opensolar_todevice");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  spiBegin();
  //setup_wifi();
  //client.setServer(mqtt_server, 1883);
  //client.setCallback(callback);
}

void loop() {

  //if (!client.connected()) {
  //  reconnect();
  //}
  //client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "hello world %ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    //client.publish("opensolar", msg);

    execute_spi_transaction();
    
    
  }
}
