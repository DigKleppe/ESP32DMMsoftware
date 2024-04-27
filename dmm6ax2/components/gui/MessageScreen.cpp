/*
 * messageScreen.cpp
 *
 *  Created on: May 9, 2021
 *      Author: dig
 */

#include "MessageScreen.h"
#include <Print.h>

static lv_obj_t * messageBox;
static TimerHandle_t timer;
static int showTimer;
static lv_style_t style;

#define BUFFERSIZE 40
static char msgBuffer[BUFFERSIZE];
static int charsInBuffer;


LV_FONT_DECLARE(lv_font_montserrat_24)

#define MSSGFONT lv_font_montserrat_24


void mssgTimerCallback (TimerHandle_t xTimer ){
	if (showTimer == 0 ){
		lv_obj_del(messageBox);
		messageBox = NULL;
		xTimerStop( xTimer, 0 );
	}
	else
		showTimer--;
}


MessageScreen::MessageScreen() { //lv_obj_t * parent, const char *text, const lv_color_t bgColor , const int msShowTime ) {

	lv_style_init(&style);

	lv_style_set_radius(&style, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_opa(&style, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_border_width(&style, LV_STATE_DEFAULT, 2);
	lv_style_set_border_color(&style, LV_STATE_DEFAULT, LV_COLOR_YELLOW);

	lv_style_set_pad_top(&style, LV_STATE_DEFAULT, 15);
	lv_style_set_pad_bottom(&style, LV_STATE_DEFAULT, 15);
	lv_style_set_pad_left(&style, LV_STATE_DEFAULT,5);
	lv_style_set_pad_right(&style, LV_STATE_DEFAULT, 5);

	lv_style_set_text_color(&style, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
	lv_style_set_text_font(&style, LV_STATE_DEFAULT,  &MSSGFONT);
	//	lv_style_set_text_letter_space(&style, LV_STATE_DEFAULT, 5);
	//	lv_style_set_text_line_space(&style, LV_STATE_DEFAULT, 20);

	timer = xTimerCreate ( "MssgTimer", 100 ,true,   ( void * ) 0, mssgTimerCallback );

	charsInBuffer =0;
}



void MessageScreen::show( const char *text, const lv_color_t bgColor , const int msShowTime )
{
	xTimerStop( timer,0 );

	if( messageBox == NULL){
		messageBox = lv_label_create(lv_scr_act(), NULL);
		lv_label_set_long_mode(messageBox,LV_LABEL_LONG_BREAK);
		lv_obj_add_style(messageBox, LV_OBJ_PART_MAIN, &style);

		lv_obj_set_width (messageBox, LV_HOR_RES_MAX);
		lv_obj_set_height(messageBox,LV_VER_RES_MAX/2);
		lv_obj_set_parent(messageBox, lv_scr_act());
		lv_style_set_bg_color(&style, LV_STATE_DEFAULT, bgColor);
	}
	lv_label_set_text(messageBox,text);
	lv_obj_set_x(messageBox, 0);
	lv_coord_t coord =lv_obj_get_height(messageBox);
	lv_obj_set_y(messageBox, (LV_VER_RES_MAX-coord)/2);
	showTimer = msShowTime/100;
	xTimerStart( timer, 0 );

}


size_t MessageScreen::write(uint8_t c)
{
    if( charsInBuffer < BUFFERSIZE) {
    	msgBuffer[charsInBuffer++]= c;
       return 1;
    }
    else
    	return 0;
}

size_t MessageScreen::write(const uint8_t *buffer, size_t size)
{
   if ( size == 0)
	   return 0;

   int m =  (int) size;
   if ( m >( BUFFERSIZE-charsInBuffer))
	   m = BUFFERSIZE-charsInBuffer;

   for (int n = 0; n < m ; n++)
	   msgBuffer[charsInBuffer++]= *buffer++;

   show( msgBuffer);
   charsInBuffer = 0;
   return (size_t) m;
}

size_t MessageScreen::printTo(Print& p) const
{
    size_t n = 0;
//    for(int i = 0; i < 3; i++) {
//        n += p.print(_address.bytes[i], DEC);
//        n += p.print('.');
//    }
//    n += p.print(_address.bytes[3], DEC);
    return n;
}

int MessageScreen::available(void) {
	return  BUFFERSIZE-charsInBuffer;

}
  int MessageScreen::peek(void) {
	  return 0;
  }

  int MessageScreen::read(void){
	  return 0;
  }
 // size_t MessageScreen::read(uint8_t *buffer, size_t size);
  void MessageScreen::flush(void)
  {};


  void MessageScreen::flush( bool txOnly)
  {};



MessageScreen::~MessageScreen() {

	// TODO Auto-generated destructor stub
}

