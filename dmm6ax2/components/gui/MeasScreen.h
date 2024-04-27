/*
 * MeasScreen.h
 *
 *  Created on: Mar 2, 2021
 *      Author: dig
 */

#ifndef COMPONENTS_GUI_MEASSCREEN_H_
#define COMPONENTS_GUI_MEASSCREEN_H_

#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif
//http://www.ltg.ed.ac.uk/~richard/utf-8.cgi?input=3a9&mode=hex

#define LV_SYMBOL_OHM    "\xef\xCE\xA9"  //0x3A9
#define LV_SYMBOL_MICRO  "\xef\xCE\xBC" //0x3BC

#include "StatusLine.h"
class MeasScreen {
public:
	MeasScreen();
	virtual ~MeasScreen();
	static const int MAXVALUECHARS = 11;
	void show();
	void setDisplayText( const char * text);
	void setValueAndName( const char * value, const char * name);
	void setStatusLine (const char * text);
	static volatile bool active;
	lv_obj_t * screen;
private:
	lv_obj_t * display;
	char  measValue1[MAXVALUECHARS+3]; // extra space for a symbol (3 characters)
	StatusLine * statusLine;
	static void event_handler(lv_obj_t * obj, lv_event_t event);


};

#endif /* COMPONENTS_GUI_MEASSCREEN_H_ */
