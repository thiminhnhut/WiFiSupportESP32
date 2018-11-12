#include "WiFiSupportESP32.h"

WiFiSupportESP32::WiFiSupportESP32() {

}

WiFiSupportESP32::WiFiSupportESP32(int8_t pin, uint8_t state) {
    _pin = pin;
    _state = state;
    pinMode(pin, OUTPUT);
}

WiFiSupportESP32::WiFiSupportESP32(Print *print, int8_t pin, uint8_t state) {
    _debug = true;
    this->_print = print;
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

            _exportInfoNetwork();

            _off();

            _debug_connect_wifi_success();

            return true;
        }

        _debug_wait_connect();
    }

    _blink();

    _debug_connect_wifi_fail();

    return false;
}

bool WiFiSupportESP32::isSmartConfig(unsigned int timeout) {
    _on();

    _readNetwork();

    _debug_ssid_password();

    bool status = false;

    if (_network.ssid.length() > 0 && _network.password.length() > 0) {
        _debug_connect_wifi();

        status = isConnected(_network.ssid.c_str(), _network.password.c_str(), timeout);

        if (status) {
            _network = _getNetwork();
            return true;
        }
    }

    if (!status) {
        _debug_start_smart_config();

        WiFi.mode(WIFI_AP_STA);
        WiFi.beginSmartConfig();

        unsigned long _start = millis();
        while (!WiFi.smartConfigDone()) {
            if (millis() - _start > timeout) {
                _blink();

                _debug_no_received_smart_config();

                return false;
            }

            _debug_wait_connect();
        }

        _debug_received_smart_config();

        while (millis() - _start <= timeout) {
            if (isConnected()) {
                _network = _getNetwork();

                _writeNetwork();

                _off();

                _exportInfoNetwork();

                _debug_smart_config_success();

                return true;
            }

            _debug_wait_connect();
        }
    }

    _blink();

    _debug_smart_config_fail();

    return false;
}

bool WiFiSupportESP32::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void WiFiSupportESP32::_exportInfoNetwork() {
    if (_debug) {
        if (isConnected()) {
            _print->println(F("\nExport Info Network"));
            _print->print(F("SSID:")); _print->println(_network.ssid);
            _print->print(F("Password:")); _print->println(_network.password);
            _print->print(F("IP:")); _print->println(_network.ip);
            _print->print(F("Getway:")); _print->println(_network.getway);
            _print->print(F("Netmask:")); _print->println(_network.netmask);
            _print->print(F("DNS:")); _print->println(_network.dns);
            _print->print(F("RSSI:")); _print->println(_network.rssi);
            _print->print(F("Mode:")); _print->println(_network.modeString);
            _print->print(F("Channel:")); _print->println(_network.channel);
            _print->print(F("BSSID:")); _print->println(_network.bssid);
            _print->print(F("Hostname:")); _print->println(_network.hostname);
        } else {
            _print->println("Connect wifi fail");
        }
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

void WiFiSupportESP32::_writeNetwork() {
    _preferences.begin(_keyWiFi, false);
    _preferences.putString(_keySSID, _network.ssid);
    _preferences.putString(_keyPassword, _network.password);
    _preferences.end();
}

void WiFiSupportESP32::_readNetwork() {
    _preferences.begin(_keyWiFi, false);
    _network.ssid = _preferences.getString(_keySSID, _defaultSSID);
    _network.password = _preferences.getString(_keyPassword, _defaultPassword);
    _preferences.end();
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
    for (unsigned char i = 0; i < 20; i++) {
        _on();
        delay(500);
        _off();
        delay(500);
    }
}

void WiFiSupportESP32::_debug_ssid_password() {
    if (_debug) {
        _print->print(F("Reading SSID and Password (Preferences):\t"));

        _print->print(F("SSID:"));
        if (_network.ssid.length() > 0) {
            _print->print(_network.ssid);
            _print->print(F("(len:")); _print->print(_network.ssid.length()); _print->print(F(")"));
        } else {
            _print->print(F("None"));
        }

        _print->print(F("\tPassword:"));
        if (_network.password.length() > 0) {
            _print->print(_network.password);
            _print->print(F("(len:")); _print->print(_network.password.length()); _print->println(F(")"));
        } else {
            _print->println(F("None"));
        }
    }
}

void WiFiSupportESP32::_debug_connect_wifi() {
    if (_debug) {
        _print->print(F("Connecting WiFi:\tSSID:")); _print->print(_network.ssid);
        _print->print(F("\tPassword:")); _print->println(_network.password);
    }
}

void WiFiSupportESP32::_debug_wait_connect() {
    if (_debug) {
        _print->print(F("."));
        delay(100);
    } else {
        delay(0);
    }
}

void WiFiSupportESP32::_debug_connect_wifi_success() {
    if (_debug) {
        _print->println(F("\nConnecting WiFi Success!"));
    }
}

void WiFiSupportESP32::_debug_connect_wifi_fail() {
    if (_debug) {
        _print->println(F("\nConnecting WiFi Failed!"));
    }
}

void WiFiSupportESP32::_debug_start_smart_config() {
    if (_debug) {
        _print->println(F("Start SmartConfig"));
    }
}

void WiFiSupportESP32::_debug_no_received_smart_config() {
    if (_debug) {
        _print->println(F("\nSmartConfig Received Failed, Timeout Error!"));
    }
}

void WiFiSupportESP32::_debug_received_smart_config() {
    if (_debug) {
        _print->println(F("\nSmartConfig received, Waiting for WiFi..."));
    }
}

void WiFiSupportESP32::_debug_smart_config_success() {
    if (_debug) {
        _print->println(F("\nSmartConfig Success!"));
    }
}

void WiFiSupportESP32::_debug_smart_config_fail() {
    if (_debug) {
        _print->println(F("\nSmartConfig Fail, Timeout error!"));
    }
}
