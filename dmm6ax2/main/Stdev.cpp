/*
 * Stdev.cpp
 *
 *  Created on: Jan 17, 2021
 *      Author: dig
 */

#include "Stdev.h"

Stdev::Stdev ( int nrValues){
	values = (float *) malloc( nrValues * sizeof( float));
	maxValues = nrValues;

}

int Stdev::addValue ( float value){
	if ( values == NULL)
		return -1;
	values[newValueIDX++] = value;
	if ( newValueIDX > maxValues)
		newValueIDX = 0;

	if ( nrValuesInBuffer < maxValues)
		nrValuesInBuffer++;
	return nrValuesInBuffer;
}

float Stdev::calcStdev (void){
	if ( nrValuesInBuffer > 1){
		float sum = 0.0, mean, SD = 0.0;
		int i;
		for (i = 0; i < nrValuesInBuffer; ++i) {
			sum += values[i];
		}
		mean = sum / 10;
		for (i = 0; i < 10; ++i)
			SD += pow(values[i] - mean, 2);
		return sqrt(SD / 10);
	}
	else
		return -1;
}




