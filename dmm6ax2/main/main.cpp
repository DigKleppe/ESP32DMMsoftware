/*

source $HOME/projecten/esp32/esp-idf/export.sh
 mkdir -p build
	cd build
	cmake .. -G Ninja   # or 'Unix Makefiles'
	ninja
 */
// compiled with idf 4.4.7
//#define USE_STATS

#include "DMM.h"
#include "DVMdisplay.h"
#include "hal/i2c_types.h"
#include "include/settings.h"
#include "MenuSettings.h"
#include "wifiConnect.h"
#include "log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_spiffs.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "Arduino.h"
#include <WiFi.h>
#include <esp_wifi.h>

//#define USE_STATS

#include "clockTask.h"
extern "C" {
#include "guiCommonTask.h"
#include "guiTask.h"
}
extern MenuSetttingsDesrc_t menuSettingsDescrTable[] ;

#define TAG "main:"
//#pragma GCC optimize ("O0")

esp_err_t start_file_server(const char *base_path);
void DMMTask(void *pvParameters);

DVMdisplay display;
volatile uint8_t relaySettings;
DMM * dmm;
bool stopWifi;
xQueueHandle keyEventQueue;
#define NO_TASKS 3
char ipAddrStr[16];
uint32_t timeStamp;

TimerHandle_t xTimer;

/* Function to initialize SPIFFS */
static esp_err_t init_spiffs(void)
{
	ESP_LOGI(TAG, "Initializing SPIFFS");

	esp_vfs_spiffs_conf_t conf = {
			.base_path = "/spiffs",
			.partition_label = NULL,
			.max_files = 25,   // This decides the maximum number of files that can be created on the storage
			.format_if_mount_failed = true
	};

	esp_err_t ret = esp_vfs_spiffs_register(&conf);
	if (ret != ESP_OK) {
		if (ret == ESP_FAIL) {
			ESP_LOGE(TAG, "Failed to mount or format filesystem");
		} else if (ret == ESP_ERR_NOT_FOUND) {
			ESP_LOGE(TAG, "Failed to find SPIFFS partition");
		} else {
			ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
		}
		return ESP_FAIL;
	}

	size_t total = 0, used = 0;
	ret = esp_spiffs_info(NULL, &total, &used);
	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
		return ESP_FAIL;
	}

	ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
	return ESP_OK;
}
static esp_err_t i2c_master_init(void)
{
	i2c_port_t i2c_master_port = (i2c_port_t) 1;   // I2C_MASTER_NUM;
	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = 21;
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_io_num = 22;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	conf.master.clk_speed = 400000;
	conf.clk_flags = 0;

	esp_err_t err = i2c_param_config(i2c_master_port, &conf);
	if (err != ESP_OK) {
		return err;
	}
	return i2c_driver_install(i2c_master_port, conf.mode, 0,0, 0);
}

void vTimerCallback( TimerHandle_t xTimer )
{
	timeStamp++;
}

 const uint32_t ulMaxExpiryCountBeforeStopping = 10;

 uint32_t ulCount;

//=============
extern "C" void app_main()
{
#ifdef USE_WIFI
	int timeout = 5;
#endif
#ifdef 	USE_STATS
	char statsBuf[500];
#endif
	TaskHandle_t taskHandles[NO_TASKS];
	ESP_ERROR_CHECK(esp_event_loop_create_default());

	gpio_set_level(ADS1256CS_PIN, 1);
	gpio_set_direction(ADS1256CS_PIN, GPIO_MODE_OUTPUT);

	ESP_ERROR_CHECK (i2c_master_init());
	ESP_ERROR_CHECK(nvs_flash_init());
	init_spiffs();
	loadSettings();

	keyEventQueue = xQueueCreate(10, sizeof(uint32_t));
	initArduino();
	Serial.begin(115200);

#ifdef USE_WIFI
	wifiConnect();
#endif
	xTaskCreatePinnedToCore(guiCommonTask, "guicommon", 4000, NULL, 0, &taskHandles[0], 1);

	while (!displayReady)// wait until LCD is ready and SPI is initialized
		vTaskDelay(100);

	xTaskCreate(clockTask, "clock", 2048, NULL, 0, NULL);

	xTaskCreatePinnedToCore(guiTask, "gui", 4000, NULL,0,&taskHandles[1], 1);
	display.printStatusLine("..Starting..");
	display.println("DMM softwareversion 3.1\n");

#ifdef USE_WIFI
	Serial.println("Connecting to WiFi..");
	display.println("Connecting to WiFi..");
#endif
	display.showMssg(1000);

   xTimer = xTimerCreate ("Timer",10 ,pdTRUE,( void * ) 0, vTimerCallback);
   xTimerStart( xTimer, 0 );

	vTaskDelay(1000);
	xTaskCreate(&DMMTask, "DMM",4000, NULL, 1, &taskHandles[2]);
#ifdef USE_WIFI
	// show network stuff
	do {
		switch (connectStatus) {
		case CONNECTING:
		case CONNECTED:
			Serial.print("Connecting to: ");
			Serial.println(WiFi.localIP());
			display.println("Connecting to:");
			display.println(wifiSettings.SSID);
			break;
		case IP_RECEIVED:
			Serial.print("ESP32 IP on the WiFi network: ");
			Serial.println(WiFi.localIP());
			display.println("Connected to:");
			display.println(wifiSettings.SSID);
			display.println("Station IP:");
			display.print(myIpAddress);
			break;
		case SMARTCONFIG_ACTIVE:
			Serial.print("Smartconfig active");
			display.println("Smartconfig active");
			display.println("Waiting for ESP touch..");
			break;
		}
		display.showMssg(2000);
		vTaskDelay(2000);
	} while ((connectStatus != IP_RECEIVED) && (timeout-- >= 0));

//	testLog( );

#endif
	while (1) {
		vTaskDelay(1000);
#ifdef 	USE_STATS
		vTaskGetRunTimeStats( statsBuf );
		Serial.print ( statsBuf);
		Serial.println("");
		for (int n = 0; n< NO_TASKS; n++){
			Serial.print("\t");
			Serial.print ( pcTaskGetName( taskHandles[n]));
			Serial.print("\t");
			Serial.print ( uxTaskGetStackHighWaterMark( taskHandles[n]));
		}
		Serial.print("\t Free:");
		Serial.println ( xPortGetFreeHeapSize());
#endif
	}
}

