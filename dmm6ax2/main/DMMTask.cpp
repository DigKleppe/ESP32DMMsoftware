/*
 * DMMTask.cpp
 *
 *  Created on: Feb 2, 2020
 *      Author: dig
 */
#include "DMM.h"
#include <stdio.h>
#include "string.h"

#include "driver/gpio.h"
#include "averager.h"
#include "keys.h"
#include "include/DMMTask.h"
#include "include/Stdev.h"
#include "include/common.h"
#include "udp.h"
#include "cgiScripts.h"
#include "wifiConnect.h"
#include "log.h"

#include <WiFi.h>

#define BUFFERSIZE 10
#define ESP_INTR_FLAG_DEFAULT 0

#define CALAC		4.0  // v RMS

#define CAL2_5mV	2.5  // calibration values reference
#define CAL25mV		23.0
#define CAL250mV	241.0
#define CAL2_5V		2.496
#define CAL5V		4.999
#define CAL50V		9.997
#define CAL500V		9.997

#define CAL2K 		1000.85
#define CAL20K 		9991.7
#define CAL200k 	100001
#define CAL2M	 	1000000
#define CAL20M	 	1000000

#define CAL10uA  	10.0
#define CAL100uA  	100.0
#define CAL1mA		1000.0
#define CAL10mA 	10.0
#define CAL100mA 	100.0
#define CAL1A		1.0
#define CAL10A		3.0

const double calFactor[] = {
CAL10uA, CAL100uA, CAL1mA, CAL10mA, CAL100mA, CAL1A, CAL10A,
CAL2_5mV, CAL25mV, CAL250mV, CAL2_5V, CAL5V, CAL50V, CAL500V,
CAL2K, CAL20K, CAL200k, CAL2M, CAL20M };

extern "C" {
extern DMM *dmm;

static xQueueHandle gpio_evt_queue = NULL;
extern volatile bool newDescriptorsReceived;

#define OVERRANGEPIN GPIO_NUM_34

static void IRAM_ATTR gpio_isr_handler(void *arg) {
	uint32_t gpio_num = (uint32_t) arg;
	xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

int waits = 1;
bool AC = false;
bool isOverRange = false;
bool autoRange = false;

// called from cgi, prints max 8k to buffer

// prints all texts and calibrationvalues

}
measValues_t measValues;  // for cgi
static log_t lastVal; // for cgi
extern bool stopWifi;
extern int scriptState;

int readDescriptors(char *pBuffer, int count) {
	int len = 0;
	len = dmm->printDescriptorHeader(pBuffer);
	*(pBuffer + len) = '\n';
	len++;

	for (int n = 0; n < NR_RANGES; n++) {
		len += dmm->printDescriptor(pBuffer + len, n);
		*(pBuffer + len) = '\n';
		len++;
	}
	return len;
}

void printStatusLine(void) {
	char str[30];
	if (autoRange)
		sprintf(str, "AR^");
	else
		str[0] = 0;
	if (dmm->getCurrentType() == type_Ohm) {
		sprintf(str + strlen(str), "%s^a:%d^%1.2fV", dmm->getCurrentDisplayText(), userSettings.averages, -dmm->getActualVoltage());

	} else {
		if (AC)
			sprintf(str + strlen(str), "%s AC a:%d", dmm->getCurrentDisplayText(), userSettings.averages);
		else
			sprintf(str + strlen(str), "%s DC a:%d", dmm->getCurrentDisplayText(), userSettings.averages);
	}

	display.printStatusLine(str);

}
void DMMKeysTask(void *pvParameters) {
	uint32_t key;

	while (1) {
		xQueueReceive(keyEventQueue, &key, portMAX_DELAY);  // key pressed
		isOverRange = false;
		waits = 3;
		switch (key) {
		case KEY_V:
			dmm->setType(type_V);
			if (dmm->getCurrentRange() == RANGE_TEMP) {
				dmm->setChannels(AN0, AN1);  // AN0 is common
				dmm->setPGA(PGA2);
			}
			break;

		case KEY_A:
			dmm->setType(type_A);
			if (dmm->getCurrentRange() == RANGE_TEMP) {
				dmm->setChannels(AN0, AN1);  // AN0 is common
				dmm->setPGA(PGA1);
			}
			break;

		case KEY_O:
			dmm->setType(type_Ohm);
			AC = false;
			if (dmm->getCurrentRange() == RANGE_TEMP)
				dmm->setChannels(AN0, AN1);  // AN0 is common
			break;

		case LV_KEY_UP:
			autoRange = false;
			dmm->setAutoRange(autoRange);
			dmm->changeRange(DIRECTION_UP);
			if (dmm->getCurrentRange() == RANGE_TEMP)
				dmm->setChannels(AN0, AN7);
			break;
		case LV_KEY_DOWN:
			autoRange = false;
			dmm->setAutoRange(autoRange);
			dmm->changeRange(DIRECTION_DOWN);
			break;

		case LV_KEY_LEFT:
			if (userSettings.precisionOffset < settingsDescr[SETTINGS_PREC].maxValue) {
				userSettings.precisionOffset++;
				saveUserSettings();
			}
			break;

		case LV_KEY_RIGHT:
			if (userSettings.precisionOffset > settingsDescr[SETTINGS_PREC].minValue) {
				userSettings.precisionOffset--;
				saveUserSettings();
			}
			break;

		case KEY_F1:
			dmm->setOffset();
			saveCalibrationSettings();
			break;

		case KEY_F2:
			AC = !AC;
			if (AC)
				dmm->setAC();
			else
				dmm->setDC();
			break;

		case KEY_F3:
			display.println("Connected to:");
			display.println(wifiSettings.SSID);
			display.println("Station IP:");
			display.print(myIpAddress);
			display.showMssg(500);
			break;

		case KEY_F4:
			autoRange = !autoRange;
			dmm->setAutoRange(autoRange);
			break;

		case LV_KEY_ENTER:

			break;

		}
		printStatusLine();
	}
}
bool isWaiting; // test
void DMMTask(void *pvParameters) {
	bool doSelfcal = false;
	double DMMvalue = 0;
	char *unit;				// text with special characters ohm etc
	char * unitFiltered; // text without special characters
	char formatStr[10];
#ifdef USE_WIFI
	char udpTxBuf[100];
#endif
	bool doSetDataRate = true;
	uint32_t gpio_num; // test only
	int formatWidth, formatPrecision;
	int oldPrecision = userSettings.precisionOffset;
	int oldAverages = userSettings.averages;
	int oldConversionSpeed = userSettings.conversionSpeed;
	int avgCntr;
	log_t logValue;

	xTaskCreate(&DMMKeysTask, "DMMKeys", 2048, NULL, 0, NULL);
	dmm = new DMM(_5SPS);

	gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
	int lastRange = -1;
	gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
	gpio_isr_handler_add(ADS1256nDRDY_PIN, gpio_isr_handler, (void*) ADS1256nDRDY_PIN);
	gpio_set_intr_type(ADS1256nDRDY_PIN, GPIO_INTR_NEGEDGE);
	gpio_set_direction(OVERRANGEPIN, GPIO_MODE_INPUT);

	dmm->setType(type_V);
	dmm->setRange(RANGE_50V);
	dmm->setAverages(userSettings.averages);
	avgCntr = userSettings.averages;

	dmm->setAutoRange(autoRange);

	dmm->setPGA(PGA2);
	vTaskDelay(5);
	dmm->setDC();

	printStatusLine();
	xQueueReceive(gpio_evt_queue, &gpio_num, 0);

	doSelfcal = true;

	while (1) {
		isWaiting = true;
		if (xQueueReceive(gpio_evt_queue, &gpio_num, 450 )) {// portMAX_DELAY)) {
			DMMvalue = dmm->read();
			isWaiting = false;
			if ( avgCntr > 0 )
				avgCntr--;
			if (lastRange != dmm->getCurrentRange()) {
				printStatusLine();
				lastRange = dmm->getCurrentRange();
			}

			if( doSetDataRate){
				dmm->setDataRate((ad1256DataRate_t) userSettings.conversionSpeed);
				doSetDataRate = false;
			}
//if (0){
//		//	if (gpio_get_level(OVERRANGEPIN) == 0) {
//				if (avgCntr ==0)
//					display.printMeasurement(OVERRANGEVALUE, NULL, NULL, DISPLAY_ITEM_MEASLINE1);
//				if (autoRange) {
//					int lastRange = dmm->getCurrentRange();
//					bool stop = false;
//					do {
//						if (dmm->changeRange(DIRECTION_UP) == lastRange) // cannot go higher
//							stop = true;
//						else {
//							vTaskDelay(20);
//							lastRange = dmm->getCurrentRange();
//						}
//					} while (!stop && gpio_get_level(OVERRANGEPIN) == 0);
//
//					if (gpio_get_level(OVERRANGEPIN) == 0) {
//						isOverRange = true;
//
//					} else
//						isOverRange = false;
//				}
//			} else {
				if (waits) // wait for autocal to finish
					waits--;
				else {
					if (doSelfcal) {
						dmm->setPGA(PGA2);
						dmm->doSelfCal();
						doSelfcal = false;
					} else {
						unit = (char*) dmm->getCurrentUnitText();
						unitFiltered = (char*) dmm->getCurrentUnitTextFiltered();  // get text without special spaces
						if (dmm->getCurrentType() == type_Ohm)
							printStatusLine(); // for printing  voltage
						if (DMMvalue < OVERRANGEVALUE) {
							dmm->getCurrentFormat(&formatWidth, &formatPrecision);
							formatPrecision += userSettings.precisionOffset;
							if (formatPrecision < 0)
								formatPrecision = 0;
							sprintf(formatStr, "%%%d.%df", formatWidth, formatPrecision);
							if (avgCntr ==0) {
								display.printMeasurement(DMMvalue, unit, formatStr, DISPLAY_ITEM_MEASLINE1);
								logValue.measValue = (float) DMMvalue;
								strcpy(logValue.unit, unitFiltered);
								addToLog(logValue);
							}
							sprintf(measValues.averegedValue, formatStr, DMMvalue);  // read by cgi
							strcat(measValues.averegedValue, " ");
							strcat(measValues.averegedValue, unitFiltered);
#ifdef USE_WIFI
							sprintf(udpTxBuf, "%f;", DMMvalue);
//							logValue.measValue = (float) DMMvalue;
//							strcpy(logValue.unit, unitFiltered);
//							addToLog(logValue);
#endif
							DMMvalue = dmm->getRealTimeValue();
							sprintf(measValues.momentaryValue, formatStr, DMMvalue);
							strcat(measValues.momentaryValue, " ");
							strcat(measValues.momentaryValue, unitFiltered);
#ifdef USE_WIFI
							sprintf(udpTxBuf + strlen(udpTxBuf), "%f;%s\n", DMMvalue, unit);
							udpSend(udpTxBuf, userSettings.udpPort);
#endif
						}

						else {
							if (DMMvalue == OVERRANGEVALUE) { // can be OVERRANGEVALUE_NO_SHOW during autoscale , suppress this
#ifdef USE_WIFI
								sprintf(udpTxBuf, "%d\n", OVERRANGEVALUE);
								udpSend(udpTxBuf, userSettings.udpPort);
#endif
								if (avgCntr ==0)
									display.printMeasurement(OVERRANGEVALUE, unit, formatStr, DISPLAY_ITEM_MEASLINE1);
							}
						}
					}
				}
			}
	//	}
		if (avgCntr ==0)
//			avgCntr = userSettings.conversionSpeed;
			avgCntr = userSettings.averages;

		if (newCalValueReceived) { // from cgi
			newCalValueReceived = false;
			dmm->calibrateCurrentRange(calValue);
			dmm->writeSettings();
		}

		if (newDescriptorsReceived) {
			newDescriptorsReceived = false;
			dmm->updateDescriptors();
			dmm->writeSettings();
		}
		bool changed = false;
		if (oldPrecision != userSettings.precisionOffset) {
			oldPrecision = userSettings.precisionOffset;
			changed = true;

		}
		if (oldAverages != userSettings.averages) {
			oldAverages = userSettings.averages;
			avgCntr = userSettings.averages;
			changed = true;
			dmm->setAverages(userSettings.averages);
		}
		if (oldConversionSpeed != userSettings.conversionSpeed) {
			oldConversionSpeed = userSettings.conversionSpeed;

			dmm->setDataRate((ad1256DataRate_t) userSettings.conversionSpeed);
			changed = true;
		}
		if (changed) {
			saveUserSettings();
			printStatusLine();
		}
	}
}

int printLog(log_t * logToPrint, char *pBuffer) {
	int len;
	len = sprintf(pBuffer, "%d,", (int) logToPrint->timeStamp);
	len += sprintf(pBuffer + len, "%f,", logToPrint->measValue);
	len += sprintf(pBuffer + len, "%s\n", logToPrint->unit);
	return len;
}

// called from CGI

int getRTMeasValuesScript(char *pBuffer, int count) {
	int len = 0;

	switch (scriptState) {
	case 0:
		scriptState++;
		len = printLog(&lastVal,pBuffer);
		return len;
		break;
	default:
		break;
	}
	return 0;
}



