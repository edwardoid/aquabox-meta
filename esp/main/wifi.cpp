#include "wifi.hpp"

#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "log.h"
extern "C" {

#include <esp_system.h>
#include <esp_netif.h>
#include <esp_wifi.h>
#include <nvs.h>
#include <nvs_flash.h>

#include <lwip/err.h>
#include <lwip/sys.h>

}

WiFiImpl WiFi;

static const char *TAG = "wifi station";

static void eventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    WiFi.handle(arg, event_base, event_id, event_data);
}

void WiFiImpl::handle(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (m_connectRetries < WIFI_CONNECT_ATTEMPTS) {
            esp_wifi_connect();
            m_connectRetries++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        } else {
            xEventGroupSetBits(m_events, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        m_ip = IPAddress(event->ip_info.ip);
        ESP_LOGI(TAG, "got ip:%s", ip4addr_ntoa(&event->ip_info.ip));
        m_connectRetries = 0;
        xEventGroupSetBits(m_events, WIFI_CONNECTED_BIT);
    }
}

const IPAddress& WiFiImpl::ip() const
{
    return m_ip;
}

void WiFiImpl::begin()
{
    m_events = xEventGroupCreate();

    tcpip_adapter_init();

    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &eventHandler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &eventHandler, NULL));
    xEventGroupClearBits(m_events, WIFI_STATE_MASK);
}

void WiFiImpl::end()
{
    ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &eventHandler));
    ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &eventHandler));
    vEventGroupDelete(m_events);
}

void WiFiImpl::scan()
{

}

void WiFiImpl::connect(const String<32>& ssid, const String<32>& password, bool wait)
{
    xEventGroupClearBits(m_events, WIFI_STATE_MASK);
    printf("Trying to connect to");
    printf("%s %s\n", ssid.cdata(), password.cdata());

    bzero(&m_config, sizeof(wifi_config_t));
    strcpy((char*) m_config.sta.ssid, ssid.cdata());
    strcpy((char*) m_config.sta.password, password.cdata());

    /* Setting a password implies station will connect to all security modes including WEP/WPA.
        * However these modes are deprecated and not advisable to be used. Incase your Access point
        * doesn't support WPA2, these mode can be enabled by commenting below line */

    if (strlen((char *)m_config.sta.password)) {
        m_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    m_connectRetries = 0;
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &m_config) );
    xEventGroupSetBits(m_events, WIFI_CONNECTING_BIT);
    ESP_ERROR_CHECK(esp_wifi_start() );

    ESP_LOGI(TAG, "wifi_init_sta finished.");

    if (wait) {
     /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
        EventBits_t bits = xEventGroupWaitBits(m_events,
                WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                pdFALSE,
                pdFALSE,
                portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
        if (bits & WIFI_CONNECTED_BIT) {
            ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                    ssid.cdata(), password.cdata());
        } else if (bits & WIFI_FAIL_BIT) {
            ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
                    ssid.cdata(), password.cdata());
        } else {
            ESP_LOGE(TAG, "UNEXPECTED EVENT");
        }
    }
}

WiFiState WiFiImpl::state() const
{
    return (WiFiState)(xEventGroupGetBits(m_events) & WIFI_STATE_MASK);
}