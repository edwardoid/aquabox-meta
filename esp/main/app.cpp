/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "wifi.hpp"

// docker run -i -t  -v $PWD/esp_rtos_libs/:/proj nodebb

extern "C" {

void connectTask(void * parameter){
    WiFi.begin();
    WiFi.connect("Sargsyans24", "azazello", false);
    vTaskDelete(NULL);
}

void wifiTackerTask(void * parameter){
    for(;;){ // infinite loop
        switch(WiFi.state()) {
            case WiFiState::Unknown: {
                break;
            }
            case WiFiState::Connecting: {
                printf("Scanning....\n");
                break;
            }
            case WiFiState::Connected: {
                printf("Finally connected!\nIP: %s\n", WiFi.ip().str().data());
                break;
            }
            case WiFiState::Scanning: {
                printf("Scanning....\n");
                break;
            }
            case WiFiState::ScanFinished: {
                printf("Finished:\n");
                for(uint8_t i = 0; i < WiFi.networks().size(); ++i)
                {
                    printf("    %s  : %d dB", WiFi.networks()[i].SSID.cdata(), WiFi.networks()[i].RSSI);
                }
                break;
            }
            case WiFiState::Failed: {
                printf("Failed!\n");
                break;
            }
            default: {
                printf("Unhandled state: %d\n", WiFi.state());
            }
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}


void app_main()
{
    printf(String<32>("Hello world!").data());
    
    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP8266 chip with %d CPU cores, WiFi, ",
            chip_info.cores);

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    xTaskCreate(
        connectTask,    // Function that should be called
        "Connect to network",  // Name of the task (for debugging)
        5000,            // Stack size (bytes)
        NULL,            // Parameter to pass
        1,               // Task priority
        NULL             // Task handle
    );

    xTaskCreate(
        wifiTackerTask,    // Function that should be called
        "NetworkTracker",  // Name of the task (for debugging)
        5000,            // Stack size (bytes)
        NULL,            // Parameter to pass
        1,               // Task priority
        NULL             // Task handle
    );

    for(;;){ // infinite loop
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    fflush(stdout);
    esp_restart();
}
}