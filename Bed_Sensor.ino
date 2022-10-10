#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

void setup() {
  Serial.begin(115200);
  wifi_connect();
  my_homekit_setup();
}

void loop() {
  my_homekit_loop();
  delay(10);
}

//==============================
// HomeKit setup and loop
//==============================

extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_contact;

#define HOMEKIT_CONTACT_SENSOR_DETECTED       0
#define HOMEKIT_CONTACT_SENSOR_NOT_DETECTED   1
#define SENSOR_PIN 2 //D4

homekit_value_t cha_programmable_switch_event_getter() {
  return HOMEKIT_NULL_CPP();
}

void my_homekit_setup() {
  arduino_homekit_setup(&config);
}

static uint32_t next_heap_millis = 0;
static uint32_t next_report_millis = 0;

void my_homekit_loop() {
  arduino_homekit_loop();
  const uint32_t t = millis();
  if (t > next_report_millis) {
    next_report_millis = t + 2 * 1000;
    my_homekit_report();
  }
  if (t > next_heap_millis) {
    next_heap_millis = t + 5 * 1000;
    LOG_D("Free heap: %d, HomeKit clients: %d",
          ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

  }
}

void my_homekit_report() {

  uint8_t value = digitalRead(SENSOR_PIN) == LOW ?
                  HOMEKIT_CONTACT_SENSOR_DETECTED : HOMEKIT_CONTACT_SENSOR_NOT_DETECTED;
  cha_contact.value.uint8_value = value;
  homekit_characteristic_notify(&cha_contact, cha_contact.value);
}
