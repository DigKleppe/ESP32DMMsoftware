/*
 * Display.cpp
 *
 *  Created on: Dec 11, 2019
 *      Author: dig
 */

#include "DVMdisplay.h"
#include "DMM.h"
#include <math.h>

#define MEASUREMENT_POSY 40


#define BUFFERSIZE 100
static char msgBuffer[BUFFERSIZE+1];
static int charsInBuffer;

extern "C"	esp_err_t initLCD(void);

DVMdisplay::DVMdisplay() {
	// 	initLCD(); / spiffs does not work when done here
	//	TFT_setFont(USER_FONT, MEASURE_FONT);
	//	measFontHeight = TFT_getfontheight();
	//	xSemaphore = xSemaphoreCreateMutex();
}

DVMdisplay::~DVMdisplay() {
}


displayMssg_t displayMssg;

void DVMdisplay::printStatusLine(const char *info)
{
	uint32_t dummy;
	displayMssg_t displayMssg;
	displayMssg.displayItem = DISPLAY_ITEM_STATUSLINE;
	displayMssg.str1 = info;
	xQueueReceive(displayReadyMssgBox, &displayMssg,0);
	if ( xQueueSend( displayMssgBox, &displayMssg, 100 )== pdPASS)
		xQueueReceive(displayReadyMssgBox, &dummy, 500); // wait until data is displayed

}

void DVMdisplay::printMeasurement ( double value, const char* unit, const char * formatStr , displayItem_t line) {

	displayMssg_t displayMssg;
	displayMssg.displayItem = line;

	char str[50];

	if( OVERRANGEVALUE == value){
		sprintf (str," OVERRANGE ");
		displayMssg.str2 = NULL;
	}
	else {
		char formatstr[20];
		sprintf( formatstr,"%s",formatStr);
		sprintf (str,formatstr,value);
		displayMssg.str2 = unit;
	}
	displayMssg.str1 = str;

	xQueueReceive(displayReadyMssgBox, &displayMssg,0);
	if ( xQueueSend( displayMssgBox, &displayMssg, 0 )== pdPASS)
		xQueueReceive(displayReadyMssgBox, &displayMssg, 500);// if accepted wait until data is displayed
	return;

}


void DVMdisplay::printMssg(char *info, int showTime)
{
	uint32_t dummy;
	displayMssg_t displayMssg;
	displayMssg.displayItem = DISPLAY_ITEM_MESSAGE;
	displayMssg.str1 = info;
	displayMssg.showTime = showTime;
	xQueueReceive(displayReadyMssgBox, &displayMssg,0);
	if ( xQueueSend( displayMssgBox, &displayMssg, 100 )== pdPASS)
		xQueueReceive(displayReadyMssgBox, &dummy, 500); // wait until data is displayed
}


size_t DVMdisplay::write(uint8_t c)
{
    if( charsInBuffer < BUFFERSIZE) {
    	msgBuffer[charsInBuffer++]= c;
       return 1;
    }
    else
    	return 0;
}

size_t DVMdisplay::write(const uint8_t *buffer, size_t size)
{
   int m =  (int) size;
   if ( m >( BUFFERSIZE-charsInBuffer))
	   m = BUFFERSIZE-charsInBuffer;

   for (int n = 0; n < m ; n++) {
	   if(*buffer != '\r' )
		   msgBuffer[charsInBuffer++]= *buffer++;
	   else
		   buffer++;
   }
   msgBuffer[charsInBuffer] = 0;
   return (size_t) m;
}

void DVMdisplay::showMssg( int showTime){
	printMssg(msgBuffer, showTime);
	charsInBuffer = 0;
}
