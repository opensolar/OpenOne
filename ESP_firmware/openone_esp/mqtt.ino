WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

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
  Serial.println("Device ID: "+ String(ESP.getChipId(), HEX));
  mqtt_client.setServer(mqtt_server, 1883);
  mqtt_client.setCallback(mqtt_on_message);
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

      String topic = "openone/" + String(ESP.getChipId(), HEX) + "/#";
      mqtt_client.subscribe(topic.c_str());
      Serial.println("subscribed to " + topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void mqtt_on_message(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // choose the motor according to topic
  MOTOR *motor;
  if(((String)topic).indexOf("pitch") != -1){
    motor = &pitch;
  } else if(((String)topic).indexOf("roll") != -1){
    motor = &roll;
  } else {
    Serial.println("Error: invalid topic");
    return;
  }

  // parse the payload
  StaticJsonDocument<64> doc;
  
  DeserializationError error = deserializeJson(doc, payload);
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  // set the motor speed
  motor->speed = doc["speed"];
  const String mode = doc["mode"];

  // set the motor mode
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




bool is_connected(){
  return mqtt_client.connected();
}

void mqtt_periodic(){
  if (!mqtt_client.connected()) {
    reconnect();
  }
  mqtt_client.loop();
}
