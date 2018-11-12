#include <Arduino.h>
#include <WiFiSupportESP32.h>

#define __SSID__ "my_ssid"
#define __PASSWORD__ "my_password"
#define __TIMEOUT__ 60000
#define __LED_STATUS__ 2
#define __LED_STATE__ HIGH

#define __SERIAL_MONITOR__ Serial
#define __BAUD_RATE__ 115200

WiFiSupportESP32 wifiSupport(&__SERIAL_MONITOR__,__LED_STATUS__, __LED_STATE__);

void setup() {
    __SERIAL_MONITOR__.begin(__BAUD_RATE__);

    bool isConnected = wifiSupport.isConnected(__SSID__, __PASSWORD__, __TIMEOUT__);

    __SERIAL_MONITOR__.println("Finish!");
}

void loop() {

}
