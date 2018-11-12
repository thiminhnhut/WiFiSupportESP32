#include "WiFiSupportESP32.h"

WiFiSupportESP32::WiFiSupportESP32() {

}

WiFiSupportESP32::WiFiSupportESP32(int8_t pin, uint8_t state) {
    _pin = pin;
    _state = state;
    pinMode(pin, OUTPUT);
}

bool WiFiSupportESP32::isConnected(const char *ssid, const char *password, unsigned int timeout) {
    _on();

    WiFi.begin(ssid, password);

    unsigned long _start = millis();
    while (millis() - _start <= timeout) {
        if (isConnected()) {
            _network = _getNetwork();
            _off();
            return true;
        } else {
            delay(0);
        }
    }

    _blink();

    return false;
}

bool WiFiSupportESP32::isSmartConfig(unsigned int address, unsigned int size, unsigned int timeout) {
    _on();

    EEPROM.begin(size);

    // _setFlagSmartConfig(address);

    _readNetwork(address);

    // Serial.println(_network.ssid);
    // Serial.println(_network.password);

    bool status = false;
    if (_network.ssid.length() > 0 && _network.password.length() > 0) {
         status = isConnected(_network.ssid.c_str(), _network.password.c_str(), timeout);
         if (status) {
             _network = _getNetwork();
             return true;
         }
    }

    if (!status) {
        WiFi.mode(WIFI_AP_STA);
        WiFi.beginSmartConfig();

        unsigned long _start = millis();
        while (!WiFi.smartConfigDone()) {
            if (millis() - _start > timeout) {
                _blink();
                return false;
            }
            delay(0);
        }

        while (millis() - _start <= timeout) {
            if (isConnected()) {
                _network = _getNetwork();
                _writeNetwork(address);
                _off();
                return true;
            }
            delay(0);
        }
    }

    _blink();

    return false;
}

bool WiFiSupportESP32::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void WiFiSupportESP32::exportInfoNetwork(Print &print) {
    if (isConnected()) {
        print.println(F("Export Info Network"));
        print.print(F("SSID:")); print.println(_network.ssid);
        print.print(F("Password:")); print.println(_network.password);
        print.print(F("IP:")); print.println(_network.ip);
        print.print(F("Getway:")); print.println(_network.getway);
        print.print(F("Netmask:")); print.println(_network.netmask);
        print.print(F("DNS:")); print.println(_network.dns);
        print.print(F("RSSI:")); print.println(_network.rssi);
        print.print(F("Mode:")); print.println(_network.modeString);
        print.print(F("Channel:")); print.println(_network.channel);
        print.print(F("BSSID:")); print.println(_network.bssid);
        print.print(F("Hostname:")); print.println(_network.hostname);
    } else {
        print.println("Connect wifi fail");
    }
}

NetWork WiFiSupportESP32::getNetwork() {
    return _network;
}

NetWork WiFiSupportESP32::_getNetwork() {
    NetWork network;

    network.ssid = _getSSID();
    network.password = _getPassword();
    network.ip = _getIP();
    network.getway = _getGetWay();
    network.netmask = _getNetMask();
    network.dns = _getDNS();
    network.rssi = _getRSSI();
    network.mode = _getMode();
    network.modeString = _getModeString();
    network.channel = _getChannel();
    network.bssid = _getBSSID();
    network.hostname = _getHostName();

    return network;
}

String WiFiSupportESP32::_getSSID() {
    return WiFi.SSID();
}

String WiFiSupportESP32::_getPassword() {
    return WiFi.psk();
}

IPAddress WiFiSupportESP32::_getIP() {
    return WiFi.localIP();
}

IPAddress WiFiSupportESP32::_getGetWay() {
    return WiFi.gatewayIP();
}

IPAddress WiFiSupportESP32::_getNetMask() {
    return WiFi.subnetMask();
}

IPAddress WiFiSupportESP32::_getDNS() {
    return WiFi.dnsIP();
}

int8_t WiFiSupportESP32::_getRSSI() {
    return WiFi.RSSI();
}

wifi_mode_t WiFiSupportESP32::_getMode() {
    return WiFi.getMode();
}

String WiFiSupportESP32::_getModeString() {
    switch (_getMode()) {
        case WIFI_MODE_STA:
            return "STA";
        case WIFI_MODE_AP:
            return "AP";
        case WIFI_MODE_APSTA:
            return "AP+STA";
        case WIFI_MODE_MAX:
            return "MAX";
        default: // WIFI_MODE_NULL
            return "NULL";
    }
}

uint32_t WiFiSupportESP32::_getChannel() {
    return WiFi.channel();
}

String WiFiSupportESP32::_getBSSID() {
    return WiFi.BSSIDstr();
}

String WiFiSupportESP32::_getHostName() {
    return WiFi.getHostname();
}

void WiFiSupportESP32::_writeNetwork(unsigned int address) {
    EEPROM.writeBool(address, true);

    unsigned int _address = address + 1;
    EEPROM.write(address, _network.ssid.length());

    _address += 1;
    EEPROM.write(_address, _network.password.length());

    _address += _network.ssid.length();
    EEPROM.writeString(_address, _network.ssid);

    _address += _network.password.length();
    EEPROM.writeString(_address, _network.password);

    EEPROM.commit();
}

void WiFiSupportESP32::_readNetwork(unsigned int address) {
    bool _connected = EEPROM.readBool(address);

    if (_connected) {
        unsigned int _address = address + 1;
        unsigned char lenSSID = EEPROM.read(address);

        _address += 1;
        unsigned char lenPassword = EEPROM.read(_address);

        _address += lenSSID;
        _network.ssid = EEPROM.readString(_address);

        _address += lenPassword;
        _network.password = EEPROM.readString(_address);
    } else {
        _network.ssid = "";
        _network.password = "";
    }
}

void WiFiSupportESP32::_setFlagSmartConfig(unsigned int address) {
    EEPROM.writeBool(address, false);
    EEPROM.commit();
}

void WiFiSupportESP32::_on() {
    if (_pin != -1) {
        digitalWrite(_pin, _state);
    }
}

void WiFiSupportESP32::_off() {
    if (_pin != -1) {
        digitalWrite(_pin, !_state);
    }
}

void WiFiSupportESP32::_blink() {
    for (unsigned char i = 0; i < 60; i++) {
        _on();
        delay(500);
        _off();
        delay(500);
    }
}
