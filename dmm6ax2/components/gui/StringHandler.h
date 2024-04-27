/*
 * StringHandler.h
 *
 *  Created on: Mar 13, 2021
 *      Author: dig
 */

#ifndef GUI_STRINGHANDLER_H_
#define GUI_STRINGHANDLER_H_

class StringHandler {
public:
	StringHandler();
	virtual ~StringHandler();
	static void setValueAndName( char * value, char * name, char *dest, int maxChars );
	static void setNameAndValue(  char * name, char * value, char *dest, int maxChars );
};

#endif /* GUI_STRINGHANDLER_H_ */
