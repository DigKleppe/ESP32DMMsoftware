/*
 * settings.c
 *
 *  Created on: Nov 30, 2017
 *      Author: dig
 */

#include "settings.h"

#include "nvs_flash.h"
#include "nvs.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "wifiConnect.h"

#include <string.h>
#include <cerrno>

#define STORAGE_NAMESPACE "storage"
#define TAG "Settings"

extern settingsDescr_t settingsDescr[];
extern char *myIpAddr;
extern int myRssi;
#define NR_DMMCALS NR_RANGES
userSettings_t userSettingsDefaults = { 0, 0, 0, 6000, { CONFIG_MDNS_HOSTNAME },
		{ USERSETTINGS_CHECKSTR } };

userSettings_t userSettings;
calibrationSettings_t calibrationSettings;


calibrationSettings_t defaultCalibrationSettings =  {
	{ -322041, -16278, -10300, -16816, -2685 ,-2756 ,-2761 ,-8930, -1, -17611 , -760, -80, -849, -1825, -123, -1785, -241, 0  }, // DC offset
	{ 4118885, 4118609, 4118050, 4074375 ,4126467 ,4126523, 4126517, 4127428, 4127397, 4125652, 4126975, 4127073, 0, 0, 0, 0, 0 }, // AC offset
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0 }, // ohmsoffset
	{ 5.234675, 52.282494, 513.675447, 4.955785, 185.622404, 1.827751, 18.523520, -1.060039, -1.029918,-2.060316,-25.538001,-258.275322, -1559.436538, -10.334858, -102.319857, -1.024442, -10.341812, 0 }, // DC gain
	1.0 , // AC gain
	CALCHECKSTR
};

settingsDescr_t settingsDescr[] = { { INT, 1,
		(void*) &userSettings.precisionOffset, -6, 2 }, { INT, 1,
		(void*) &userSettings.averages, 1, 32 }, { INT, 1,
		(void*) &userSettings.conversionSpeed, 1, 16 }, { INT, 1,
		(void*) &userSettings.udpPort, 1, 65500 }, { STR, 0, (void*) 0, 0, 0 } // last size 0
};

settingsDescr_t calibrationSettingsDesrc[] = { { INT, NR_DMMCALS,
		(void*) calibrationSettings.DCoffset, 0, 0 }, { INT, NR_DMMCALS,
		(void*) calibrationSettings.ACoffset, 0, 0 }, { INT, NR_DMMCALS,
		(void*) calibrationSettings.ohmsOffset, 0, 0 }, { FLT, NR_DMMCALS,
		(void*) calibrationSettings.calFactor, 0, 0 },
		{ STR, 0, (void*) 0, 0, 0 } // last size 0
};


esp_err_t saveUserSettings(void) {
	nvs_handle_t my_handle;
	esp_err_t err = 0;

	err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));
	} else {
		err |= nvs_set_blob(my_handle, "userSettings",(const void *) &userSettings, sizeof(userSettings_t));

		switch (err) {
		case ESP_OK:
			ESP_LOGI(TAG, "settings written");
			break;
		default:
			ESP_LOGE(TAG, "Error (%s) writing!", esp_err_to_name(err));
		}
		err = nvs_commit(my_handle);

		switch (err) {
		case ESP_OK:
			ESP_LOGI(TAG, "Committed");
			break;
		default:
			ESP_LOGE(TAG, "Error (%s) commit", esp_err_to_name(err));
		}
		nvs_close(my_handle);
	}
	return err;
}


esp_err_t saveSettings(void) {
	nvs_handle_t my_handle;
	esp_err_t err = 0;

	err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));
	} else {
		err = nvs_set_blob(my_handle, "WifiSettings",(const void *) &wifiSettings, sizeof(wifiSettings_t));
		err |= nvs_set_blob(my_handle, "userSettings",(const void *) &userSettings, sizeof(userSettings_t));

		switch (err) {
		case ESP_OK:
			ESP_LOGI(TAG, "settings written");
			break;
		default:
			ESP_LOGE(TAG, "Error (%s) writing!", esp_err_to_name(err));
		}
		err = nvs_commit(my_handle);

		switch (err) {
		case ESP_OK:
			ESP_LOGI(TAG, "Committed");
			break;
		default:
			ESP_LOGE(TAG, "Error (%s) commit", esp_err_to_name(err));
		}
		nvs_close(my_handle);
	}
	return err;
}

esp_err_t loadSettings() {
	nvs_handle_t my_handle;
	esp_err_t err = 0;
	bool doSave = false;

	err = nvs_open(STORAGE_NAMESPACE, NVS_READONLY, &my_handle);
	size_t len;
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));
	} else {
		ESP_LOGI(TAG, "reading SSID and password");
		len =  sizeof(wifiSettings_t);
		err = nvs_get_blob(my_handle, "WifiSettings", (void *) &wifiSettings, &len);
		len = sizeof(userSettings_t);
		err |= nvs_get_blob(my_handle, "userSettings",(void *) &userSettings, &len);
		switch (err) {
		case ESP_OK:
			ESP_LOGI(TAG, "OTABootSSID: %s", wifiSettings.SSID);
			break;
		case ESP_ERR_NVS_NOT_FOUND:
			ESP_LOGE(TAG, "The value is not initialized yet!");
			break;
		default:
			ESP_LOGE(TAG, "Error (%s) reading!", esp_err_to_name(err));
		}
		nvs_close(my_handle);
	}

#ifdef CONFIG_FIRMWARE_UPGRADE_FILENAME
	if (strcmp(wifiSettings.upgradeFileName, CONFIG_FIRMWARE_UPGRADE_FILENAME) != 0) {
		strcpy(wifiSettings.upgradeFileName, CONFIG_FIRMWARE_UPGRADE_FILENAME);
		doSave = true;  // set filename for OTA via factory firmware
	}
#endif

	if(strncmp(userSettings.checkstr,USERSETTINGS_CHECKSTR, strlen (USERSETTINGS_CHECKSTR) )	!= 0)
	{
		userSettings = userSettingsDefaults;
		wifiSettings = wifiSettingsDefaults;
		ESP_LOGE(TAG, "default usersettings loaded");
		doSave = true;  // set filename for OTA via factory firmware
	}

	if ( doSave)
		return saveSettings();
	else {
		ESP_LOGI(TAG, "usersettings loaded");
	}
	return err;

}

esp_err_t saveCalibrationSettings(void) {
	FILE *fd = fopen("/spiffs/calsettings", "wb");
	if (fd == NULL) {
		printf("  Error opening file (%d) %s\n", errno, strerror(errno));
		printf("\n");
		return -1;
	}
	strcpy(calibrationSettings.checkstr, CALCHECKSTR);

	int len = sizeof(calibrationSettings_t);
	int res = fwrite(&calibrationSettings, 1, len, fd);
	if (res != len) {
		printf("  Error writing to file(%d <> %d\n", res, len);
		res = fclose(fd);
		return -1;
	}
	res = fclose(fd);
	return 0;
}

esp_err_t loadCalibrationSettings() {
	esp_err_t res = 0;
	FILE *fd = fopen("/spiffs/calsettings", "rb");
	if (fd == NULL) {
		printf("  Error opening settings file (%d) %s\n", errno,
				strerror(errno));

	} else {
		int len = sizeof(calibrationSettings_t);
		res = fread(&calibrationSettings, 1, len, fd);
		if (res <= 0) {
			printf("  Error reading from file\n");
		}
		res = fclose(fd);
	}
	if (strcmp(calibrationSettings.checkstr, CALCHECKSTR) != 0) {
		calibrationSettings = defaultCalibrationSettings;
		printf(" ** defaults loaded");
		saveCalibrationSettings();
	}
	return res;
}
