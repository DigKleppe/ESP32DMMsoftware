/*
 * log.h
 *
 *  Created on: Nov 3, 2023
 *      Author: dig
 */

#ifndef MAIN_INCLUDE_LOG_H_
#define MAIN_INCLUDE_LOG_H_

#include <stdint.h>
#include <time.h>


#define MAXLOGVALUES	4*1024

typedef struct {
	uint32_t timeStamp; // in 10ms
	float measValue;
	char unit[4];
} log_t;

extern int logRxIdx;
extern int logTxIdx;
extern uint32_t timeStamp;

int getAllLogsScript(char *pBuffer, int count);
int getNewLogsScript(char *pBuffer, int count);
void addToLog( log_t logValue);


#endif /* MAIN_INCLUDE_LOG_H_ */
