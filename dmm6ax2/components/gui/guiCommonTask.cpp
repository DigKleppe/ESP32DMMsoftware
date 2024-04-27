/*
 * guiTask.c
 *
 *  Created on: Mar 2, 2021
 *      Author: dig
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "../lvgl_esp32_drivers/lvgl_helpers.h"
#include "mpr121.h"
#include "keys.h"

#include "MeasScreen.h"
#include "guiTask.h"
#include "Arduino.h"

// fonts Ohm, micro  0x3A9, 0x3BC, 0x5E, 0xB0 01234567890 -.,mMnkVAHzOVERNGE
//https://lvgl.io/tools/fontconverter
#define LV_TICK_PERIOD_MS 30

extern "C" {
/* Creates a semaphore to handle concurrent call to lvgl stuff
 * If you wish to call *any* lvgl function from other threads/tasks
 * you should lock on the very same semaphore! */
SemaphoreHandle_t xGuiSemaphore;
static void lv_tick_task(void *arg);
lv_indev_drv_t indev_drv;
lv_indev_t *kb_indev;
lv_indev_data_t keydata;
volatile bool displayReady;

QueueHandle_t displayMssgBox;
QueueHandle_t displayReadyMssgBox;

void initKeyboardDriver();
uint8_t i2cErr = 0;

MPR121 * mpr121;


void guiCommonTask(void *pvParameter) {

	(void) pvParameter;
	TickType_t xLastWakeTime;

	mpr121 = new MPR121((uint8_t) MPR_ADDRESS, (i2c_port_t)1);
	xGuiSemaphore = xSemaphoreCreateMutex();

	//esp_err_t err = i2c_master_init();
	lv_init();

	/* Initialize SPI or I2C bus used by the drivers */
	lvgl_driver_init();

	lv_color_t* buf1 = (lv_color_t*)heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
	assert(buf1 != NULL);

	/* Use double buffered when not working with monochrome displays */
#ifndef CONFIG_LV_TFT_DISPLAY_MONOCHROME
	lv_color_t* buf2 = (lv_color_t*)heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
	assert(buf2 != NULL);
#else
	static lv_color_t *buf2 = NULL;
#endif

	static lv_disp_buf_t disp_buf;

	uint32_t size_in_px = DISP_BUF_SIZE;

#if defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_IL3820         \
		|| defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_JD79653A    \
		|| defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_UC8151D     \
		|| defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_SSD1306

	/* Actual size in pixels, not bytes. */
	size_in_px *= 8;
#endif

	/* Initialize the working buffer depending on the selected display.
	 * NOTE: buf2 == NULL when using monochrome displays. */
	lv_disp_buf_init(&disp_buf, buf1, buf2, size_in_px);

	lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv);

	disp_drv.flush_cb = disp_driver_flush;

	/* When using a monochrome display we need to register the callbacks:
	 * - rounder_cb
	 * - set_px_cb */
#ifdef CONFIG_LV_TFT_DISPLAY_MONOCHROME
	disp_drv.rounder_cb = disp_driver_rounder;
	disp_drv.set_px_cb = disp_driver_set_px;
#endif

	disp_drv.buffer = &disp_buf;
	lv_disp_drv_register(&disp_drv);
	initKeyboardDriver();

	/* Register an input device when enabled on the menuconfig */
#if CONFIG_LV_TOUCH_CONTROLLER != TOUCH_CONTROLLER_NONE
	lv_indev_drv_t indev_drv;
	lv_indev_drv_init(&indev_drv);
	indev_drv.read_cb = touch_driver_read;
	indev_drv.type = LV_INDEV_TYPE_POINTER;
	lv_indev_drv_register(&indev_drv);
#endif

	/* Create and start a periodic timer interrupt to call lv_tick_inc */
	const esp_timer_create_args_t periodic_timer_args = {
			.callback = &lv_tick_task,
			.name = "periodic_gui"
	};
	esp_timer_handle_t periodic_timer;
	ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
	ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));
//	lv_disp_set_rotation()

	displayMssgBox = xQueueCreate(1, sizeof(displayMssg_t));
	displayReadyMssgBox = xQueueCreate(1, sizeof(uint32_t));

	displayReady = true;

	while (1) {

		vTaskDelayUntil( &xLastWakeTime, LV_TICK_PERIOD_MS );
	//	vTaskDelay(30);  // klp

		/* Try to take the semaphore, call lvgl related function on success */
		if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY)) {
			lv_task_handler();
			xSemaphoreGive(xGuiSemaphore);
		}
	}

	/* A task should NEVER return */
	free(buf1);
#ifndef CONFIG_LV_TFT_DISPLAY_MONOCHROME
	free(buf2);
#endif
	vTaskDelete(NULL);
}


static void lv_tick_task(void *arg) {
	(void) arg;

	lv_tick_inc(LV_TICK_PERIOD_MS);

	///	keydata.key = mpr121.readTouchBits();
}

static uint32_t keycodeToAscii(uint32_t key){

	switch( key){
	case KEY_RIGHT:
		return LV_KEY_RIGHT;

	case KEY_LEFT:
		return LV_KEY_LEFT;

	case KEY_UP:
		return LV_KEY_UP;

	case KEY_DOWN:
		return LV_KEY_DOWN;

		/*    case SDLK_ESCAPE:
        return LV_KEY_ESC;*/

#ifdef  LV_KEY_BACKSPACE        /*For backward compatibility*/
	case SDLK_BACKSPACE:
		return LV_KEY_BACKSPACE;
#endif

#ifdef  LV_KEY_DEL        /*For backward compatibility*/
	case SDLK_DELETE:
		return LV_KEY_DEL;
#endif

	case KEY_SET:
	case '\r':
		return LV_KEY_ENTER;
	default:
		return key;
	}
}

bool readKeys(struct _lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
	static uint32_t lastKey;
	uint32_t key = mpr121->readTouchBits();

	if( key){
		lastKey = keycodeToAscii(key);
		data->state = LV_INDEV_STATE_PR;          /*Save the key is pressed now*/
	}
	else                          /*Button release*/
		data->state = LV_INDEV_STATE_REL;         /*Save the key is released but keep the last key*/
	data->key = lastKey;
	return false;
}

void initKeyboardDriver (void){

	lv_indev_drv_init(&indev_drv);
	indev_drv.type =LV_INDEV_TYPE_KEYPAD;
	indev_drv.read_cb = readKeys;
	kb_indev = lv_indev_drv_register(&indev_drv);
}
}
