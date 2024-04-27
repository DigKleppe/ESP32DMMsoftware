/*
 * Display.h
 *
 *  Created on: Dec 11, 2019
 *      Author: dig
 */

#ifndef MAIN_DVMDISPLAY_H_
#define MAIN_DVMDISPLAY_H_

extern "C" {
	#include "lvgl.h"
}

//#include "../../../arduinoLib/arduinoCores/Print.h"
#include "Print.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "guiCommonTask.h"
#include "guiTask.h"
#include "Print.h"

class DVMdisplay : public Print {

public:
	DVMdisplay();
	virtual ~DVMdisplay();
	void printStatusLine(const char *info);
	void printMssg(char *info, int showTime = 2000);
	void showMssg( int showTime = 3000);
	void printMeasurement ( double value, const char* unit, const char* formatStr , displayItem_t line);
	SemaphoreHandle_t xSemaphore = NULL;

    size_t write(uint8_t);
    size_t write(const uint8_t *buffer, size_t size);

private:
	int measFontHeight;
};

#endif /* MAIN_DVMDISPLAY_H_ */
