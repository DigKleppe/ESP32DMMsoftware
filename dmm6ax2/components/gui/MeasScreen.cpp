

/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdlib.h>
#include <unistd.h>
#include "MeasScreen.h"
#include "common.h"
#include "guiCommonTask.h"

// fonts Ohm, micro  0x3A9,0x3BC 01234567890 -.,mMnkVAHz

LV_FONT_DECLARE(insolata60);
LV_FONT_DECLARE(insolata70);
LV_FONT_DECLARE(insolata75);


#define FONT &insolata75
#define PADDING 5


static lv_style_t measStyle1;

static lv_obj_t * screen;
static lv_obj_t * display;
static lv_group_t * g;
volatile bool MeasScreen::active;

#define LV_COLOR_BGBLUE   LV_COLOR_MAKE(1, 0, 31)
lv_color_t bg;

MeasScreen::MeasScreen() {

	lv_style_init(&measStyle1);
	lv_color_t bg;
	bg.full = 7937;

	lv_style_set_bg_color(&measStyle1, LV_STATE_DEFAULT,bg);
	lv_style_set_radius(&measStyle1, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_opa(&measStyle1, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_border_width(&measStyle1, LV_STATE_DEFAULT, 2);
	lv_style_set_border_color(&measStyle1, LV_STATE_DEFAULT, LV_COLOR_WHITE);

	lv_style_set_pad_top(&measStyle1, LV_STATE_DEFAULT, 30);
	lv_style_set_pad_bottom(&measStyle1, LV_STATE_DEFAULT, PADDING+4);
	lv_style_set_pad_left(&measStyle1, LV_STATE_DEFAULT, PADDING);
	lv_style_set_pad_right(&measStyle1, LV_STATE_DEFAULT, PADDING);

	lv_style_set_text_color(&measStyle1, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
	lv_style_set_text_letter_space(&measStyle1, LV_STATE_DEFAULT, 1);

	lv_style_set_text_font(&measStyle1, LV_STATE_DEFAULT, FONT);

	screen = lv_obj_create(NULL, NULL);

	display = lv_label_create(screen, NULL);
	lv_label_set_long_mode(display,LV_LABEL_LONG_CROP);
	lv_obj_add_style(display, LV_OBJ_PART_MAIN, &measStyle1);
	lv_label_set_align(display, LV_LABEL_ALIGN_CENTER);       /*Center aligned lines*/
	lv_obj_set_width(display, LV_HOR_RES_MAX);
	//lv_coord_t fontHeight = lv_font_get_line_height(FONT);
	//lv_obj_set_height(display,fontHeight + 2* PADDING);

	statusLine = new StatusLine ( screen);
	lv_obj_set_height(display,LV_HOR_RES_MAX - lv_obj_get_height ((lv_obj_t *) &statusLine));

	//	lv_obj_align(display, NULL, LV_ALIGN_CENTER, 0,5);
	g = lv_group_create();
	lv_obj_set_event_cb(screen, event_handler);
	lv_group_add_obj(g, screen);
	lv_obj_set_state(screen,  LV_STATE_FOCUSED);
}

void MeasScreen::event_handler(lv_obj_t * obj, lv_event_t event)
{
	uint32_t key;
	if ( event == LV_EVENT_KEY ){
		key = lv_indev_get_key(kb_indev);
		if ( key == LV_KEY_ENTER)
			active = false;
		else
			xQueueSend(keyEventQueue, &key, 0);  // send key to dmm task
	}
}

void MeasScreen::setDisplayText(const  char * text) {
	int len = strlen( text);

	if ( len >= MAXVALUECHARS )
		memcpy( measValue1,text, MAXVALUECHARS);
	else {
		memcpy( measValue1,text,len);
		for (int n = len; n< MAXVALUECHARS; n++)

			measValue1[MAXVALUECHARS] = 0;
	}

	lv_label_set_text(display, measValue1);
}

/**
 * @brief
 * @param value
 * @param name
 */
void MeasScreen::setValueAndName(  const char * value, const char * name) {
	bool hasSymbol = false;
	int valLen = strlen(value);
	//	int nameLen = strlen(name);
	//	if (nameLen > MAXVALUECHARS)
	//		nameLen = MAXVALUECHARS;
	int nameLen = 0;
	if ( name != NULL ){
		do {
			if((uint8_t) name[nameLen] == 0xef)
				hasSymbol = true;
			nameLen++;

		} while( (uint8_t) name[nameLen] != 0);
	}

	if ( nameLen == 0)
		lv_label_set_text(display, value);
	else {
		if (hasSymbol) {  // assumend only 1 symbol
			memcpy ( &measValue1[MAXVALUECHARS+2-nameLen],name,nameLen); // set value at last positions of display
			nameLen -=2;  // subtract 2 extra characters for symbol
		}
		else
			memcpy ( &measValue1[MAXVALUECHARS -nameLen],name,nameLen); // set value at last positions of display

		int pos = MAXVALUECHARS-nameLen-valLen;
		if ( pos < 0) {
			valLen += pos; // does not fit
			pos = 0;
		}
		for ( int n = 0; n < pos; n++){
			measValue1[n] = ' '; // fill spaces at first part of line
		}
		memcpy(  &measValue1[pos],value, valLen);
		if (hasSymbol)
			measValue1[MAXVALUECHARS+2] = 0;
		else
			measValue1[MAXVALUECHARS] = 0;
		lv_label_set_text(display, measValue1);
	}
}

void MeasScreen::setStatusLine (const char * text){
	statusLine->setText(text);
}

void MeasScreen::show(){
	lv_scr_load(screen);
	lv_indev_set_group(kb_indev ,g);
	active = true;
}


MeasScreen::~MeasScreen() {
	// TODO Auto-generated destructor stub
}

void makeMeasScreen( void){


}




