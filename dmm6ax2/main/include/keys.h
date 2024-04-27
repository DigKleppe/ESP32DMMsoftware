/*
 * Keys.h
 *
 *  Created on: Feb 2, 2020
 *      Author: dig
 */

#ifndef MAIN_INCLUDE_KEYS_H_
#define MAIN_INCLUDE_KEYS_H_

/*
 *
 * 	12		11	  	10		9
 *
 * 	8		7		6		5
 *
 * 	2		1		3		4
 *
 */

#define KEY_V 			(1<<11)
#define KEY_A 			(1<<7)
#define KEY_O 			(1<<1)

#define KEY_UP 			(1<<9)
#define KEY_DOWN 		(1<<2)
#define KEY_LEFT 		(1<<6)
#define KEY_RIGHT 		(1<<4)
#define KEY_SET			(1<<5)

#define KEY_F1 			(1<<10) // F1
#define KEY_F2 			(1<<8)  // F2
#define KEY_F3 			(1<<0)	// F3
#define KEY_F4 			(1<<3)	// F4



#endif /* MAIN_INCLUDE_KEYS_H_ */
