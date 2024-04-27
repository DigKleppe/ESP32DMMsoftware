/*
 * Stdev.h
 *
 *  Created on: Jan 17, 2021
 *      Author: dig
 */

#ifndef MAIN_INCLUDE_STDEV_H_
#define MAIN_INCLUDE_STDEV_H_
#include <math.h>

class Stdev  {

public:
	Stdev(int nrValues);
	int addValue( float val);
	float calcStdev( void);

private:
	float * values;
	int newValueIDX = 0;;
	int maxValues;
	int nrValuesInBuffer=0;

};




#endif /* MAIN_INCLUDE_STDEV_H_ */
