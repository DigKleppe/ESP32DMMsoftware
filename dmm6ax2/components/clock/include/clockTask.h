/*
 * clock.h
 *
 *  Created on: Apr 2, 2022
 *      Author: dig
 */

#ifndef GUI_CLOCKTASK_H_
#define GUI_CLOCKTASK_H_

#include <time.h>
#include <sys/time.h>

void clockTask(void *pvParameter);
extern bool clockIsSet;


#endif /* GUI_CLOCKTASK_H_ */
