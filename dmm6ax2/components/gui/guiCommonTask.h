/*
 * guiTask.h
 *
 *  Created on: Apr 14, 2021
 *      Author: dig
 */

#ifndef COMPONENTS_GUI_GUICOMMONTASK_H_
#define COMPONENTS_GUI_GUICOMMONTASK_H_

#include "lvgl.h"

extern "C" {

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

void guiCommonTask(void *pvParameter);
extern lv_indev_t *kb_indev;

extern QueueHandle_t displayMssgBox;
extern QueueHandle_t displayReadyMssgBox;
extern SemaphoreHandle_t xGuiSemaphore;
extern volatile bool displayReady;
}

#endif /* COMPONENTS_GUI_GUICOMMONTASK_H_ */
