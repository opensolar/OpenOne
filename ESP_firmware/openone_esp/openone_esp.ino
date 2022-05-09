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


// Example of credentials.h
//const char* ssid = "my_ssid";
//const char* password = "my_password";
//const char* mqtt_server = "technolaf.com:1883";

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


unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
uint32_t free_counter = 0;

static MOTOR roll;
static MOTOR pitch;
static CONTROL ctrl;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  
  digitalWrite(BUILTIN_LED, LOW);

  // keep reset HIGH
  pinMode(16, OUTPUT);  
  digitalWrite(16, HIGH);
 
  
  Serial.begin(115200);
  spiBegin();
  setup_wifi();
}


void loop() {
  uint32_t now;

  mqtt_periodic();
  if(!is_connected()){
    roll.mode = COAST;
    pitch.mode = COAST;
  } 

  ctrl.digit[0] = 11;
  ctrl.digit[1] = 22;
  ctrl.bits = RUN;
  
  
  now = millis();
  if (now - lastMsg > 20) {
    lastMsg = now;
    ++free_counter; 

    // print every 1.28s    
    if(free_counter % 64 == 0){
      Serial.println("pitch speed:" + String(pitch.speed) + " mode:" + String(pitch.mode));
      Serial.println("roll speed:" + String(roll.speed) + " mode:" + String(roll.mode));

      Serial.print("halls: ");
      for(int i = 0; i < NB_HALLS; i++){
        Serial.print(String(get_hall_sensor(i))+ " ");
      }
      Serial.print("\n");
      Serial.println("ST status:" + String(get_st_status()));
    

      
    }
    
    if(free_counter % 2){
      ctrl.watchdog =0x55;
    } else {
      ctrl.watchdog =0xAA;
    }
    
    execute_spi_transaction(&roll, &pitch, &ctrl);
    
    
  }
}
