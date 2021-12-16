#ifndef WIFI_HPP
#define WIFI_HPP

#ifndef WIFI_CONNECT_ATTEMPTS  
#define WIFI_CONNECT_ATTEMPTS 30
#endif // WIFI_CONNECT_ATTEMPTS

extern "C" {
#include <stdint.h>
#include <esp_event.h>
#include <freertos/event_groups.h>
}
#include "string.hpp"
#include "ip_address.hpp"
#include "array.hpp"

#define WIFI_CONNECTING_BIT BIT0
#define WIFI_CONNECTED_BIT  BIT1
#define WIFI_FAIL_BIT       BIT2
#define WIFI_SCANING_BIT    BIT3
#define WIFI_SCAN_FINISHED  BIT4

#define WIFI_STATE_MASK (WIFI_CONNECTING_BIT | WIFI_CONNECTED_BIT | WIFI_FAIL_BIT | WIFI_SCANING_BIT | WIFI_SCAN_FINISHED)

struct WiFiNetwork
{
    String<32> SSID;
    int16_t RSSI;
};

enum WiFiState
{
    Unknown = 0,
    Connecting = WIFI_CONNECTING_BIT,
    Connected = WIFI_CONNECTED_BIT,
    Failed = WIFI_FAIL_BIT,
    Scanning = WIFI_SCANING_BIT,
    ScanFinished = WIFI_SCAN_FINISHED
};

class WiFiImpl
{
public:
    WiFiImpl() {}
private:
    WiFiImpl(const WiFiImpl&) {}
    const WiFiImpl& operator = (const WiFiImpl&) { return *this; }
public:
    void begin();
    void end();
    const IPAddress& ip() const;
    void connect(const String<32>& ssid, const String<32>& password, bool wait = false);
    void scan();
    inline const Array<WiFiNetwork>& networks() { return m_networks; }

    WiFiState state() const;

public: // !!! INTERNAL !!!
    void handle(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
private:
    EventGroupHandle_t m_events;
    wifi_config_t m_config;
    IPAddress m_ip;
    Array<WiFiNetwork> m_networks;
    uint8_t m_connectRetries;
};

extern WiFiImpl WiFi;

#endif // WIFI_HPP