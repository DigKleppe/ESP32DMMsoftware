#include <math.h>
#include <stdint.h>

#include "ntc.h"

#define RREF 10000
#define BVALUE 3977


#define ADC_FULL_SCALE 0x7FFFFF

#pragma GCC optimize ("0")

float calcNTC( uint32_t adcVal ){
	float fRntc;
//	fRntc = 10000 *  (float) adcVal / ( ADC_FULL_SCALE  - (float) adcVal);
	fRntc = 10000 *  ( ADC_FULL_SCALE  - (float) adcVal) /(float) adcVal ;
	return(fRntc);
}

float calcTemp( float fRntc){
	float fTemp;
	fTemp = (1.0 / ( (log(fRntc/RREF))/BVALUE  + 1.0/298.0)) - 273.0; //log = ln
	return( fTemp);
}

// returnwaarde: temperatuur in graden C	
// input sensor 

float readTemp(uint32_t adcVal){
	float fRntc,fTemp;
	fRntc = calcNTC(adcVal);
	fTemp = calcTemp (fRntc);
	return(fTemp);
}

