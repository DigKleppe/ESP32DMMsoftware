/*
 * Clock.cpp
 *
 *  Created on: Apr 2, 2022
 *      Author: dig
 */

#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_sntp.h"

bool clockIsSet;

static const char *TAG = "Clock";

static void initialize_sntp(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();
}

void clockTask(void *pvParameter) {
	char strftime_buf[64];
	struct tm timeinfo;
    setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3",1);
    tzset();

    initialize_sntp();

    time_t now = 0;
    int retry = 0;
    const int retry_count = 20;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET  && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... );// (%d/%d)", retry, retry_count);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
	time(&now);
   	localtime_r(&now, &timeinfo);
   	strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
   	ESP_LOGI(TAG, "The current date/time is: %s", strftime_buf);
   	clockIsSet = true;
   	vTaskDelete(NULL);
}



