#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"

#define uS_IN_A_SECOND   1000000ULL
#define uS_IN_AN_HOUR    (3600ULL * uS_IN_A_SECOND)
#define MAX_MQTT_RETRIES 3
#define WIFI_TIMEOUT_MS  20000

RTC_DATA_ATTR int bootCount = 0;
WiFiClient espClient;
PubSubClient client(espClient);

void connectToWiFi();
bool reconnectMqtt();
void measureData();
bool sendDataToBroker();

void setup() {
  Serial.begin(115200);
  delay(100);
  
  esp_sleep_enable_timer_wakeup(6 * uS_IN_AN_HOUR);
  bootCount++;
  
  Serial.printf("\nBoot #%d\n", bootCount);
  
  connectToWiFi();
  
  client.setServer(MQTT_SERVER, 1883);
  if(!reconnectMqtt()) {
    Serial.println("MQTT failed, sleeping...");
    goto sleep;
  }

  measureData();
  
  if(!sendDataToBroker()) {
    Serial.println("Data send failed");
  }

sleep:
  client.disconnect();
  WiFi.disconnect(true);
  Serial.println("Entering deep sleep");
  esp_deep_sleep_start();
}

void loop() {}

// Implementierungen
void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - start > WIFI_TIMEOUT_MS) {
      Serial.println(" WiFi timeout!");
      esp_deep_sleep_start();
    }
    delay(200);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

bool reconnectMqtt() {
  for(int i=0; i<MAX_MQTT_RETRIES; i++) {
    if (client.connect("ESP32Client")) {
      return true;
    }
    Serial.printf("MQTT fail (%d), retry %d/%d\n", 
                client.state(), i+1, MAX_MQTT_RETRIES);
    delay(2000);
  }
  return false;
}

void measureData() {
  sensorData = analogRead(34);
  // Plausibilitätscheck
  if(sensorData < 100 || sensorData > 4095) {
    Serial.println("Invalid sensor reading!");
    sensorData = -1;
  }
}

bool sendDataToBroker() {
  if(sensorData == -1) return false;

  char topic[30];
  char payload[40];
  
  snprintf(topic, sizeof(topic), "sensors/%s/soil", DEVICE_ID);
  snprintf(payload, sizeof(payload), "{\"value\":%d}", sensorData);

  bool success = client.publish(topic, payload);
  
  // Warte auf Sendung
  for(int i=0; i<50; i++) {
    if(client.loop()) break;
    delay(10);
  }
  
  return success;
}