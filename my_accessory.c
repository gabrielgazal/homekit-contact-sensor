#include <homekit/homekit.h>
#include <homekit/characteristics.h>

void my_accessory_identify(homekit_value_t _value) {
  printf("accessory identify\n");
}

homekit_characteristic_t cha_contact = HOMEKIT_CHARACTERISTIC_(CONTACT_SENSOR_STATE, 0);

homekit_accessory_t *accessories[] = {
  HOMEKIT_ACCESSORY(.id = 1, .category = homekit_accessory_category_sensor, .services = (homekit_service_t*[]) {
    HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics = (homekit_characteristic_t*[]) {
      HOMEKIT_CHARACTERISTIC(NAME, "Contact Sensor"),
      HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Gabriel Gazal"),
      HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "20041999-10102022"),
      HOMEKIT_CHARACTERISTIC(MODEL, "Contact Sensor - WEMOS D1"),
      HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
      HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
      NULL
    }),
    HOMEKIT_SERVICE(CONTACT_SENSOR, .primary = true, .characteristics = (homekit_characteristic_t*[]) {
      &cha_contact,
      NULL
    }),
    NULL
  }),
  NULL
};

homekit_server_config_t config = {
  .accessories = accessories,
  .password = "111-11-111"
};
