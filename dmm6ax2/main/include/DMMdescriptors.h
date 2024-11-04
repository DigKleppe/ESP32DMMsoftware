/*
 * DVMdescriptors.h
 *
 *  Created on: Jan 19, 2020
 *      Author: dig
 */

#ifndef MAIN_DMMDESCRIPTORS_H_
#define MAIN_DMMDESCRIPTORS_H_


//#include "Ads1256.h"
#include "common.h"
#include "Switches.h"


typedef enum {
    RANGE_10uA,RANGE_100uA, RANGE_1mA ,RANGE_10mA, RANGE_100mA, RANGE_11A, RANGE_20A,
	RANGE_250mV, RANGE_2_5V, RANGE_5V, RANGE_50V,RANGE_500V,
	RANGE_2k, RANGE_20k, RANGE_200k, RANGE_2M, RANGE20M,RANGE_TEMP, NR_RANGES } range_t;

typedef enum { type_A = 'A', type_V ='V', type_Ohm = 'O' , type_temp = 'T', type_NotUsed ='N'} type_t;


typedef struct {
	const type_t type;
	const char unitText[6];
	const char unitTextFiltered[4];  // unitText without special chars for http
	const char rangeText[12];
	int width;
	int precision;
	uint32_t switchSettings;
	int32_t DCoffset;
	int32_t ACoffset;
	double DCgain;
	double ACgain;
} DMMdescriptor_t ;


class DMMdescriptors {
public:
	DMMdescriptors( void );
	virtual ~DMMdescriptors();
	int setRange( int newRange);
	int setOverRange(void);
	int changeRange ( direction_t dir );
	int setType (type_t newType);
	void setDC( void);
	void setAC( void);
	const char* getCurrentDisplayText (void);
	const char* getCurrentUnitText (void);
	const char* getCurrentUnitTextFiltered (void);
	void getCurrentFormat (int *with, int* prec);
	double getCurrentRef(void);
	int32_t getCurrentDCoffset(void);
	int32_t getCurrentACoffset(void);
	int setParameters(uint32_t DCoffset, uint32_t  ACoffset, double DCgain, double ACgain, uint32_t idx);
	double getCurrentDCGain(void);
	double getCurrentACGain(void);
	int setACGain ( double  ACoffset,uint32_t idx );
	int printDescriptor( char * buffer ,int idx);
	int printDescriptorHeader( char * buffer );
	int getCurrentRange(void);
	type_t getCurrentType( void);
	void setCurrentDCoffset(int32_t);
	void setCurrentACoffset(int32_t);
	void setCurrentGain(double);
	void loadDCoffsets( int32_t * p);
	void loadACoffsets( int32_t * p);
	void loadCalFactors( double * p);
	bool DC =true;
	bool isMaxRange = false;

	const char descriptorHeader[21] = {"DMMdescriptors v1.0"};

private:
	void writeSwitches( void);
	Switches switches; // to control frontend
	uint32_t switchSettings;
	const char * descriptorItemNames[7] = {"type","unit","range","DCoffset","ACoffset","DCgain","ACgain"};
	int currentIdx = 0;

	DMMdescriptor_t dmmDescriptorTable[NR_RANGES] = {
		{type_A, "^" LV_SYMBOL_MICRO "A",	"uA",	"10" LV_SYMBOL_MICRO "A",	5,4, SW_10uA, 0 ,0, 1.0 , 1.0  },
		{type_A, "^" LV_SYMBOL_MICRO "A", 	"uA",	"100" LV_SYMBOL_MICRO "A",	5,3, SW_100uA, 0 ,0, 1.0 , 1.0  },
		{type_A, "^" LV_SYMBOL_MICRO "A",	"uA", 	"1mA",						5,3 ,SW_1mA,0 , 0,1.0 , 1.0  },
		{type_A, "^mA",						"mA",	"10mA", 					5,3, SW_10mA, 0 ,0, 1.0 , 1.0  },
		{type_A, "^mA",						"mA",	"100mA", 					6,4, SW_100mA,0 ,0, 1.0 , 1.0  },
		{type_A ,"^A",						"A",  	"1A",						6,3, SW_1A,0 ,0, 1.0  , 1.0 },
		{type_A, "^A",						"A", 	"10A",						6,3, SW_10A,0 , 0,1.0  , 1.0 },
		{type_V, "^mV",   					"mV",	"250mV",					6,2, SW_250mV, 0 , 0,1.0 , 1.0  },
		{type_V, "^V",	 					"V",	"2.5V" 	, 					7,3, SW_2_5V, 0 ,0, 1.0 , 1.0  },
		{type_V, "^V",						"V", 	"5V", 						6,3, SW_5V, 0 ,0,1.0 , 1.0  },
		{type_V, "^V",						"V", 	"50V", 						6,3, SW_50V, 0 ,0, 1.0 , 1.0  },
		{type_V, "^V",						"V", 	"500V", 					6,2, SW_500V, 0 ,0, 1.0 , 1.0  },
		{type_Ohm, "^"  LV_SYMBOL_OHM,		"E",	"2k" LV_SYMBOL_OHM,			5,2, SW_2kOhm, 0 , 0,1.0 , 1.0  },
		{type_Ohm, "^k" LV_SYMBOL_OHM, 		"k",	"20k" LV_SYMBOL_OHM, 		6,2, SW_20kOhm, 0 , 0,1.0 , 1.0  },
		{type_Ohm, "^k" LV_SYMBOL_OHM, 		"k", 	"200k" LV_SYMBOL_OHM, 		6,2, SW_200kOhm, 0 , 0,1.0 , 1.0  },
		{type_Ohm, "^M" LV_SYMBOL_OHM, 		"M",	"20M" LV_SYMBOL_OHM, 		6,2, SW_20MOhm, 0 ,0, 1.0 , 1.0 } ,
		{type_temp, "^" LV_SYMBOL_DEGREE "C",		"C", LV_SYMBOL_DEGREE "C",	4,2, SW_TEMP, 0 ,0, 1.0 , 1.0  }
	};




};

#endif /* MAIN_DMMDESCRIPTORS_H_ */
