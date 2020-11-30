//  _____   _____  _______ __   _ _______  _____         _______  ______   _____  _____ 
// |     | |_____] |______ | \  | |______ |     | |      |_____| |_____/     |   |     |
// |_____| |       |______ |  \_| ______| |_____| |_____ |     | |    \_ . __|__ |_____|

// Author: Martin Lafleur (mlafleur@opensolar.io)
// Date: Sep 2020
// Project: OpenOne
//
// Copyright 2020 opensolar.io
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and 
// associated documentation files (the "Software"), to deal in the Software without restriction, 
// including without limitation the rights to use, copy, modify, merge, publish, distribute, 
// sublicense, and/or sell copies of the Software, and to permit persons to whom the Software 
// is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in 
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//   

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include "credentials.h"

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
  uint8_t watchdog;
} CONTROL;
enum control_bits { RED_LED = 0x02, RUN = 0x01 };

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

static MOTOR roll;
static MOTOR pitch;
static CONTROL ctrl;

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

void mqtt_on_message(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  
  MOTOR *motor;
  if(((String)topic).indexOf("pitch") != -1){
    motor = &pitch;
  } else if(((String)topic).indexOf("roll") != -1){
    motor = &roll;
  } else {
    Serial.println("Error: no motor is found in topic");
    return;
  }
  
  StaticJsonDocument<64> doc;
  //{"speed": 65535, "mode":"CCW"}
  //if(topic == pitch)
  DeserializationError error = deserializeJson(doc, payload);
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
  motor->speed = doc["speed"];
  const String mode = doc["mode"];
  
  if(mode == "CCW"){
    motor->mode = CCW;
  } else if(mode == "CW"){
    motor->mode = CW;
  } else if(mode == "BRAKE"){
    motor->mode = BRAKE;
  } else if(mode == "COAST"){
    motor->mode = COAST;
  } else {
    Serial.println("Error: motor mode is not supported");
    motor->mode = COAST;
  }
  
  

}

void reconnect() {
  // Loop until we're reconnected
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (mqtt_client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // ... and resubscribe
      mqtt_client.subscribe("openone/#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  digitalWrite(BUILTIN_LED, LOW);
  
  Serial.begin(115200);
  spiBegin();
  
  setup_wifi();
  mqtt_client.setServer(mqtt_server, 1883);
  mqtt_client.setCallback(mqtt_on_message);
}





void loop() {

  if (!mqtt_client.connected()) {
    roll.mode = COAST;
    pitch.mode = COAST;
    reconnect();
  }
  mqtt_client.loop();

  ctrl.digit[0] = 11;
  ctrl.digit[1] = 22;
  ctrl.bits = RUN;
  
  unsigned long now = millis();
  if (now - lastMsg > 20) {
    lastMsg = now;
    ++value; 

    // print every 1.28s    
    if(value % 64 == 0){
      Serial.println("pitch speed:" + String(pitch.speed) + " mode:" + String(pitch.mode));
      Serial.println("roll speed:" + String(roll.speed) + " mode:" + String(roll.mode));
    }
    
    if(value % 2){
      ctrl.watchdog =0x55;
    } else {
      ctrl.watchdog =0xAA;
    }
    
    execute_spi_transaction(&roll, &pitch, &ctrl);
    
    
  }
}
