################################
Thư viện kết nối WiFi cho ESP32
################################

:Info: Github <https://github.com/thiminhnhut/WiFiSupportESP32>
:Author: Thi Minh Nhựt <thiminhnhut@gmail.com>
:Date: $Date: 11/11/2018 $
:Revision: $Revision: 1.2.0 $
:Description: Thư viện kết nối WiFi cho ESP32

===============================================================

.. sectnum::

.. contents:: Nội dung

===============================================================

Nội dung thực hiện
******************

Thư viện WiFiSupportESP32
=============================

* Thư mục source: `src <https://github.com/thiminhnhut/WiFiSupportESP32/blob/master/src>`_.

  * Struct ``NetWork`` gồm các field sau:

  .. code::

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

  * File `WiFiSupportESP32.h <https://github.com/thiminhnhut/WiFiSupportESP32/blob/master/src/WiFiSupportESP32.h>`_:

  .. code::

    WiFiSupportESP32();

    WiFiSupportESP32(int8_t pin, uint8_t state);

    WiFiSupportESP32(Print *print, int8_t pin, uint8_t state);

    bool isConnected(const char* ssid, const char* password, unsigned int timeout);

    bool isSmartConfig(unsigned int timeout);

    bool isConnected();

    NetWork getNetwork();

  * File `WiFiSupportESP32.cpp <https://github.com/thiminhnhut/WiFiSupportESP32/blob/master/src/WiFiSupportESP32.cpp>`_.

* Ví dụ:

  * Kết nối WiFi cho ESP32 `ConnectWiFi.ino <https://github.com/thiminhnhut/WiFiSupportESP32/blob/master/examples/ConnectWiFi/ConnectWiFi.ino>`_

  * Kết nối WiFi cho ESP32 với giao thức SmartConfig `SmartConfig.ino <https://github.com/thiminhnhut/WiFiSupportESP32/blob/master/examples/SmartConfig/SmartConfig.ino>`_

Chức năng của thư viện WiFiSupportESP32
===========================================

* Kết nối WiFi cho ESP32 với ``SSID`` và ``Password``.

* Kết nối WiFi cho ESP32 với giao thức SmartConfig (Đọc thông tin ``SSID`` và ``Password`` từ ``NVR`` để kết nối lại với mạng WiFi, khắc phục bug không nhớ ``SSID`` và ``Password`` sau khi ``SmartConfig``).

* Lấy thông tin của mạng WiFi đang kết nối (``struct NetWork``).

Revision
========

* `Version 1.0.0 <https://github.com/thiminhnhut/WiFiSupportESP32/releases/tag/1.0.0>`_ (Sử dụng ``EEPROM`` để lưu thông tin ``SSID`` và ``Password``).

* `Version 1.1.0 <https://github.com/thiminhnhut/WiFiSupportESP32/releases/tag/1.1.0>`_ (Sử dụng phân vùng ``NVS`` để lưu thông tin ``SSID`` và ``Password``).

* `Version 1.2.0 <https://github.com/thiminhnhut/WiFiSupportESP32/releases/tag/1.2.0>`_ (Đọc thông tin ``SSID`` và ``Password`` từ ``NVR``).
