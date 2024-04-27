/*
 * messageScreen.h
 *
 *  Created on: May 9, 2021
 *      Author: dig
 */

#ifndef COMPONENTS_GUI_MESSAGESCREEN_H_
#define COMPONENTS_GUI_MESSAGESCREEN_H_
#include "lvgl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#include <Print.h>
//#include "Stream.h"

class MessageScreen: public Print{
public:
	MessageScreen();
	virtual ~MessageScreen();
	void show( const char *text, const lv_color_t bgColor =LV_COLOR_BLACK , const int msShowTime = 1000 );
    size_t write(uint8_t);
    size_t write(const uint8_t *buffer, size_t size);
    virtual size_t printTo(Print& p) const;
    int available(void);
      int peek(void);
      int read(void);
    //  size_t read(uint8_t *buffer, size_t size);
      void flush(void);
      void flush( bool txOnly);



private:
	//lv_obj_t * messageBox;
	//TimerHandle_t timer;


};

#endif /* COMPONENTS_GUI_MESSAGESCREEN_H_ */
