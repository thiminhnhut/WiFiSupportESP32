#include <Arduino.h>
#include <WiFiSupportESP32.h>

#define __TIMEOUT__ 120000
#define __ADDRESS_INFO_NETWORK__ 0x00
#define __SIZE_EEPROM__ 1024
#define __LED_STATUS__ 2
#define __LED_STATE__ HIGH

#define __SERIAL_MONITOR__ Serial
#define __BAUD_RATE__ 115200

WiFiSupportESP32 wifiSupport(__LED_STATUS__, __LED_STATE__);

void setup() {
    __SERIAL_MONITOR__.begin(__BAUD_RATE__);

    bool isConnected = wifiSupport.isSmartConfig(__ADDRESS_INFO_NETWORK__, __SIZE_EEPROM__, __TIMEOUT__);

    wifiSupport.exportInfoNetwork(__SERIAL_MONITOR__);

    __SERIAL_MONITOR__.println("Finish!");
}

void loop() {

}
