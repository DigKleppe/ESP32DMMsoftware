/*
 * AmpereMeter.h
 *
 *  Created on: Dec 11, 2019
 *      Author: dig
 */

#ifndef DMM_H
#define DMM_H
#include "common.h"
#include "Ads1256.h"
#include "Ads1256.h"
#include "DMMdescriptors.h"

#include <mpr121.h>
#include <DVMdisplay.h>
#include "averager.h"


#define USE_WIFI

#define ADCFULLSCALE 8317356  // should be 8388608 ?
#define OVERRANGEVALUE			999999999
#define OVERRANGEVALUE_NO_SHOW  9999999999

extern DVMdisplay display;


class DMM: public DMMdescriptors {
//class DMM {
public:
	DMM(ad1256DataRate_t datarate);
	virtual ~DMM();
	float setDataRate( ad1256DataRate_t dataRateIdx);
	void measure(void);
	void* setAverages(uint32_t);
	int setRange(int newRange);
	void setAutoRange( bool);
	bool getAutoRange (void);
	int rangeUp(void);
	int rangeDown(void);
	//void setFilter( bool on);
	void setAC(void);
	void setDC(void);

	int changeRange(direction_t dir);
	ads1256PGA_t setPGA(ads1256PGA_t);
	ads1256PGA_t changePGA(direction_t dir);
	ads1256PGA_t getPGA(void);
	void doSelfCal(void);
	adsResult_t setChannels(ads1256Channel_t posChannel,ads1256Channel_t negChannel);
	double read(void);
	double getRealTimeValue(void);
	double getActualVoltage(void);
	uint32_t getRawValue();
	void setOffset(void);
	void calibrateCurrentRange( double calValue);
	void writeSettings( void);
	int updateDescriptors (void);


private:
	Ads1256 *ads1256;
	bool autoRange = false;
	double volt = 0;
	double realTimeValue;
	double offset;
	double actualVoltage;
	bool calcOffset = false;
	uint32_t rawValue;
	ads1256PGA_t PGA = PGA1;
	int mode = 0;
	Averager averager;
	Averager refAverager;
	int averages = 15;
	bool autoPGA = false;
	bool AC = false;

	int32_t adcFullScale = ADCFULLSCALE;

	double calcResistance(int32_t adcValue);
//	DMMdescriptors *dmmDescriptors;
};

#endif /* MAIN_AMPEREMETER_H_ */
