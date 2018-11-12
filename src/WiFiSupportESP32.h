#ifndef WiFiSupportESP32_h
#define WiFiSupportESP32_h

#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>

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

    bool _debug = false;
    Print *_print;

    Preferences _preferences;
    const char* _keyWiFi = "wifi";
    const char* _keySSID = "ssid";
    const char* _keyPassword = "password";
    String _defaultSSID = "";
    String _defaultPassword = "";

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

    void _exportInfoNetwork();

    void _writeNetwork();
    void _readNetwork();

    void _on();
    void _off();
    void _blink();

    void _debug_ssid_password();
    void _debug_connect_wifi();
    void _debug_wait_connect();
    void _debug_connect_wifi_success();
    void _debug_connect_wifi_fail();
    void _debug_start_smart_config();
    void _debug_no_received_smart_config();
    void _debug_received_smart_config();
    void _debug_smart_config_success();
    void _debug_smart_config_fail();

public:

    WiFiSupportESP32();

    WiFiSupportESP32(int8_t pin, uint8_t state);

    WiFiSupportESP32(Print *print, int8_t pin, uint8_t state);

    bool isConnected(const char* ssid, const char* password, unsigned int timeout);

    bool isSmartConfig(unsigned int timeout);

    bool isConnected();

    NetWork getNetwork();

};
#endif
