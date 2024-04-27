/*
 * DVMdescriptors.cpp
 *
 *  Created on: Jan 19, 2020
 *      Author: dig
 */

#include "DMMdescriptors.h"
#include <stdio.h>

extern volatile uint8_t switchSettings;

int lastRangeV= 10,lastRangeA = 6,lastRangeO= -1;  // idx from ddmmdescriptor table

#define NO_DMMDESCR (sizeof (dmmDescriptorTable)/ sizeof( DMMdescriptor_t))

DMMdescriptors::DMMdescriptors( void ) {
}

int DMMdescriptors::printDescriptor( char * buffer ,int idx){
	int len;
	len = sprintf (buffer,"%c," , dmmDescriptorTable[idx].type);
	len +=  sprintf (buffer+len,"%s," , dmmDescriptorTable[idx].unitTextFiltered);
	len +=  sprintf (buffer+len,"%s," , dmmDescriptorTable[idx].rangeText);
	len +=  sprintf (buffer+len,"%d," , dmmDescriptorTable[idx].DCoffset);
	len +=  sprintf (buffer+len,"%d," , dmmDescriptorTable[idx].ACoffset);
	len +=  sprintf (buffer+len,"%f," , dmmDescriptorTable[idx].DCgain);
	len +=  sprintf (buffer+len,"%f," , dmmDescriptorTable[idx].ACgain);
	return len;
}

int DMMdescriptors::printDescriptorHeader( char * buffer ){
	int len =0;
	len = sprintf( buffer, "DMMdescriptors v1.0\n");
	for ( int n = 0 ; n< ( sizeof (descriptorItemNames)/ sizeof ( char *)); n++) {
		len += sprintf (buffer+len,"%s," ,descriptorItemNames[n]);
	}
	return len;

}
void DMMdescriptors::writeSwitches( void) {
	switchSettings = dmmDescriptorTable[currentIdx].switchSettings;
	switches.write(switchSettings);
}
// returns index to dvmDescriptorTable
//int DMMdescriptors::changeRange (input_t input, int currentRange, direction_t dir ){

int DMMdescriptors::changeRange(direction_t dir) {
	int newIdx = currentIdx;
	type_t currentType = dmmDescriptorTable[currentIdx].type;
	switch (dir) {
	case DIRECTION_UP:
		if (currentIdx < NO_DMMDESCR)
			newIdx++;
		break;
	case DIRECTION_DOWN:
		isMaxRange = false;
		if (currentIdx > 0)
			newIdx--;
		break;

	default:
		break;
	}

	if (dmmDescriptorTable[newIdx].type != currentType) { // still this input_t?
		if ( dir == DIRECTION_UP) // set to maximum range (for autoranging)
			isMaxRange = true;
		return currentIdx; // no , do not change
	}
	switch (currentType) {
	case type_A:
		lastRangeA = newIdx; // remember for changing type
		break;
	case type_V:
		lastRangeV = newIdx; // remember for changing type
		break;
	case type_Ohm:
		lastRangeO = newIdx; // remember for changing type
		break;
	default:
		break;
	}
	isMaxRange = false;
	currentIdx = newIdx;
	writeSwitches();
	return newIdx;
}

int DMMdescriptors::setRange( int newRange) {
	if (newRange < NR_RANGES){
		if ( currentIdx == newRange)
			return newRange;
		currentIdx = newRange;
		writeSwitches();
		return newRange;
	}
	else
		return -1;
}


int DMMdescriptors::setOverRange( void){
	switch (dmmDescriptorTable[currentIdx].type ){

	case type_A:
		setRange(RANGE_20A);
		break;
	case type_V:
		setRange(RANGE_500V);
		break;
	case type_Ohm:
	case type_temp:
	case type_NotUsed:
		break;
	}
	return currentIdx;
}

int DMMdescriptors::getCurrentRange( void) {
	return currentIdx;

}
int32_t DMMdescriptors::getCurrentDCoffset(void) {
	return dmmDescriptorTable[currentIdx].DCoffset;
}

int32_t DMMdescriptors::getCurrentACoffset(void) {
	return dmmDescriptorTable[currentIdx].ACoffset;
}


double DMMdescriptors::getCurrentDCGain(void) {
	return dmmDescriptorTable[currentIdx].DCgain;
}
double DMMdescriptors::getCurrentACGain(void) {
	return dmmDescriptorTable[currentIdx].ACgain;
}

const char * DMMdescriptors::getCurrentDisplayText(void) {
	return dmmDescriptorTable[currentIdx].rangeText;
}

const char* DMMdescriptors::getCurrentUnitText (void) {
	return dmmDescriptorTable[currentIdx].unitText;
}
const char* DMMdescriptors::getCurrentUnitTextFiltered (void) {
	return dmmDescriptorTable[currentIdx].unitTextFiltered;
}

const char* getCurrentUnitTextFiltered (void);
void DMMdescriptors::getCurrentFormat (int *width, int* prec) {
	*width = dmmDescriptorTable[currentIdx].width;
	*prec = dmmDescriptorTable[currentIdx].precision;
}


type_t DMMdescriptors::getCurrentType( void){
	return dmmDescriptorTable[currentIdx].type;
}

void DMMdescriptors::setCurrentDCoffset(int32_t offset){
	dmmDescriptorTable[currentIdx].DCoffset = offset;
}
void DMMdescriptors::setCurrentACoffset(int32_t offset){
	dmmDescriptorTable[currentIdx].ACoffset = offset;
}
int  DMMdescriptors::setACGain ( double  gain,uint32_t idx ){
	if ( idx < NR_RANGES) {
		dmmDescriptorTable[idx].ACgain = gain;
		return 0;
	}
	return -1;
}

int DMMdescriptors::setParameters(uint32_t DCoffset, uint32_t  ACoffset, double DCgain, double ACgain, uint32_t idx) {
	if ( idx < NR_RANGES) {
		dmmDescriptorTable[idx].DCoffset = DCoffset;
		dmmDescriptorTable[idx].ACoffset = ACoffset;
		dmmDescriptorTable[idx].DCgain = DCgain;
		dmmDescriptorTable[idx].ACgain = ACgain;
		return 0;
	}
	return -1;

}


void DMMdescriptors::setCurrentGain(double calFactor){
	dmmDescriptorTable[currentIdx].DCgain = calFactor;
}

// returns index to dvmDescriptorTable and sets the relays
int DMMdescriptors::setType(type_t newType) {
	int newIdx = 0;
	for (newIdx = 0; newIdx < NO_DMMDESCR; newIdx++) {
		if (dmmDescriptorTable[newIdx].type == newType)
			break;
	}
	currentIdx = newIdx;
	// override if previous type is used
	switch (newType) {
	case type_A:
		if (lastRangeA >=0)
			currentIdx = lastRangeA; // set to previous range for A
		break;
	case type_V:
		if (lastRangeV >=0)
			currentIdx = lastRangeV; // set to previous range for V
		break;
	case type_Ohm:
		if (lastRangeO >=0)
			currentIdx = lastRangeO; // set to previous range for Ohms
		break;
	default:
		break;
	}
	writeSwitches();

	return newIdx;
}

void DMMdescriptors::setDC( void) {
	if (DC)
		return;
	else {
		DC = true;
		writeSwitches();
	}
}


void DMMdescriptors::setAC( void) {
	if (!DC)
		return;
	else {
		DC = false;
		writeSwitches();
	}
}


void DMMdescriptors::loadDCoffsets( int32_t * p) {
	for (int n= 0; n < NO_DMMDESCR; n++)
		dmmDescriptorTable[n].DCoffset = *p++;
}

void DMMdescriptors::loadACoffsets( int32_t * p) {
	for (int n= 0; n < NO_DMMDESCR; n++)
		dmmDescriptorTable[n].ACoffset = *p++;
}


void DMMdescriptors::loadCalFactors( double * p) {
	for (int n= 0; n < NO_DMMDESCR; n++)
		dmmDescriptorTable[n].DCgain = *p++;
}


DMMdescriptors::~DMMdescriptors() {
	// TODO Auto-generated destructor stub
}

