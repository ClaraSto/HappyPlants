#include  <Arduino.h>
#include "esp_sleep.h"

/*In the main method, the esp32 is put to the deep sleep mode. 
  It wakes up twice a day to meassure the humidity of the soil.
  Then the ESP32 sends the data to a Raspberry Pi with MQTT. */

#define uS_IN_A_SECOND   1000000ULL
#define uS_IN_A_MINUTE   (60ULL * uS_IN_A_SECOND)
#define uS_IN_AN_HOUR    (60ULL * uS_IN_A_MINUTE)

#define TIME_TO_SLEEP  6           // Time ESP32 will go to sleep (in seconds)

int bootCount; 

void setup() {


    Serial.begin(115200);  // Start serial communication
    delay(1000);  //Take some time to open up the Serial Monitor
     esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_IN_AN_HOUR);


  bootCount++;
   Serial.println("BootCount: " + bootCount);

  //TODO: Messure humidity

  //TODO: Send Data to Raspberry Pi

  Serial.println("Gehe in den Deep-Sleep-Modus für 6 Stunden");
  esp_deep_sleep_start();
}

void loop() {
  //never reached
}
