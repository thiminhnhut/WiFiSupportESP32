#ifndef WiFiSupportESP32_h
#define WiFiSupportESP32_h

#include <Arduino.h>
#include <WiFi.h>
#include <EEPROM.h>

struct NetWork {
    String ssid;
    String password;
    IPAddress ip;
    IPAddress getway;
    IPAddress netmask;
    IPAddress dns;
    int8_t rssi;
    wifi_mode_t mode;
    String modeString;
    uint32_t channel;
    String bssid;
    String hostname;
};

class WiFiSupportESP32 {

private:

    int8_t _pin = -1;
    uint8_t _state;

    NetWork _network;
    NetWork _getNetwork();
    String _getSSID();
    String _getPassword();
    IPAddress _getIP();
    IPAddress _getGetWay();
    IPAddress _getNetMask();
    IPAddress _getDNS();
    int8_t _getRSSI();
    wifi_mode_t _getMode();
    String _getModeString();
    uint32_t _getChannel();
    String _getBSSID();
    String _getHostName();

    void _writeNetwork(unsigned int address);
    void _readNetwork(unsigned int address);
    void _setFlagSmartConfig(unsigned int address);

    void _on();
    void _off();
    void _blink();

public:

    WiFiSupportESP32();

    WiFiSupportESP32(int8_t pin, uint8_t state);

    bool isConnected(const char* ssid, const char* password, unsigned int timeout);

    bool isSmartConfig(unsigned int address, unsigned int size, unsigned int timeout);

    bool isConnected();

    NetWork getNetwork();

    void exportInfoNetwork(Print &print);

};
#endif
