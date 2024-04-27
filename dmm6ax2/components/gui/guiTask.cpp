/*
 * guiTask.c
 *
 *  Created on: Mar 2, 2021
 *      Author: dig
 *
 *      handles screens
 */

#include "MeasScreen.h"
#include "MenuSettings.h"
#include "MessageScreen.h"
#include "guiTask.h"
#include "guiCommonTask.h"
extern "C" {

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
//#include  "disp_spi.h"

extern MenuSetttingsDesrc_t DMMSettingsDescrTable[];
void disp_wait_for_pending_transactions(void);

userState_t userState = USER_STATE_RUN;

void guiTask(void *pvParameter) {
	displayMssg_t recDdisplayMssg;
	int dummy;

	MeasScreen measScreen;
	MenuSettings menuSettings;
	MessageScreen messageScreen;
	measScreen.show();

	while (1) {
		if(	xQueueReceive(displayMssgBox, &recDdisplayMssg, 0 ) == pdTRUE) {
			//portMAX_DELAY);
			if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY)) {
				switch (recDdisplayMssg.displayItem) {
				case DISPLAY_ITEM_STATUSLINE:
					measScreen.setStatusLine((const char *)recDdisplayMssg.str1);
					break;

				case DISPLAY_ITEM_MEASLINE1:
					measScreen.setValueAndName((const char *)recDdisplayMssg.str1, (const char *)recDdisplayMssg.str2);    //," "  LV_SYMBOL_MICRO "V");
					break;
				case DISPLAY_ITEM_MEASLINE2:
					break;
				case DISPLAY_ITEM_STOP:
					break;

				case DISPLAY_ITEM_MESSAGE:
					messageScreen.show ((const char *) recDdisplayMssg.str1 , LV_COLOR_BLACK, recDdisplayMssg.showTime);
				break;
				}
				disp_wait_for_pending_transactions();
				xSemaphoreGive(xGuiSemaphore);
			}
			xQueueSend(displayReadyMssgBox, &dummy, 0);

		}

		switch ( userState){

		case USER_STATE_RUN:
			if( measScreen.active) {

			}
			else {
				userState = USER_STATE_MENU; // enter pressed while in run state , activate menu
				menuSettings.show(&DMMSettingsDescrTable[0]);
			}
			break;

		case USER_STATE_MENU:
			if ( menuSettings.active == false){
				userState = USER_STATE_RUN;
				measScreen.show();
			}


			break;
		default:
			break;

		}

		vTaskDelay(10);

	}

}
}

// /home/dig/.espressif/tools/openocd-esp32/v0.10.0-esp32-20200420/openocd-esp32/bin/openocd -f interface/ftdi/c232hm.cfg -f board/esp-wroom-32.cfg -c "program_esp /home/dig/projecten/littleVGL/dmmGui/build/dmm. 0x10000 verify"
