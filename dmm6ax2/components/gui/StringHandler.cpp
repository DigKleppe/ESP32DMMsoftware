/*
 * StringHandler.cpp
 *
 *  Created on: Mar 13, 2021
 *      Author: dig
 */

#include <StringHandler.h>
#include <string.h>
#include <stdint.h>


StringHandler::StringHandler() {
	// TODO Auto-generated constructor stub

}
/**
 * @brief sets value and name , value left aligned, name right.
 * 			handles one UTF8 symbol in name . eg 123,2 uV
 *
 * @param value pointer to value
 * @param name	pointer to name
 * @param dest	pointer to destination buffer, should be maxChars+1 in size
 * @param maxChars
 * @return
 */
void StringHandler::setValueAndName( char * value, char * name, char *dest, int maxChars ){
	bool hasSymbol = false;
	int valLen = strlen(value);
	int nameLen = strlen(name);
	if (nameLen > maxChars)
		nameLen = maxChars;

	for (int n = 0 ; n < nameLen; n++) {
		if((uint8_t) name[n] == 0xef)
			hasSymbol = true;
	}
	if (hasSymbol) {  // assumend only 1 symbol
		memcpy ( &dest[maxChars+2-nameLen],name,nameLen); // set name at last positions of display
		nameLen -=2;  // subtract 2 extra characters for symbol
	}
	else
		memcpy ( &dest[maxChars -nameLen],name,nameLen);

	int pos = maxChars-nameLen-valLen;
	if ( pos < 0) {
		valLen += pos; // does not fit
		pos = 0;
	}
	for ( int n = 0; n < pos; n++){
		dest[n] = ' '; // fill spaces at first part of line
	}
	memcpy(  &dest[pos],value, valLen);
	if (hasSymbol)
		dest[maxChars+2] = 0;
	else
		dest[maxChars] = 0;
}


void StringHandler::setNameAndValue(  char * name, char * value, char *dest, int maxChars ){

	int valLen = strlen(value);
	int nameLen = strlen(name);
	if (nameLen > maxChars)
		nameLen = maxChars;

	memset(dest,  ' ' ,maxChars);

	memcpy ( &dest[maxChars -valLen],value,valLen); // set value at last positions of display
	memcpy ( dest , name, nameLen);
	*(dest+maxChars) = 0;

}


StringHandler::~StringHandler() {
	// TODO Auto-generated destructor stub
}

