/*
 * common.h
 *
 *  Created on: Jan 20, 2020
 *      Author: dig
 */

#ifndef MAIN_INCLUDE_COMMON_H_
#define MAIN_INCLUDE_COMMON_H_

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"


typedef enum { INPUT_V, INPUT_A, INPUT_OHM } input_t;
typedef enum { RESULT_OK, RESULT_ERROR } result_t;
typedef enum { DIRECTION_NONE, DIRECTION_UP, DIRECTION_DOWN } direction_t;

extern "C" xQueueHandle keyEventQueue;

#define LV_SYMBOL_OHM    "\xef\xCE\xA9"  //0x3A9
#define LV_SYMBOL_MICRO  "\xef\xCE\xBC" //0x3BC
#define LV_SYMBOL_DEGREE "\xef\xCE\xB0"


#endif /* MAIN_INCLUDE_COMMON_H_ */
