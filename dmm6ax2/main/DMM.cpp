/*
 * AmpereMeter.cpp
 *
 *  Created on: Dec 11, 2019
 *      Author: dig
 */

#include "DMM.h"
#include <stdint.h>
#include <float.h>
#include "include/settings.h"
#include "include/ntc.h"
#include <string.h>

ads1256Regs_t ads1256RegsCpy;

DMM::DMM(ad1256DataRate_t datarate) {
	loadCalibrationSettings();

	loadCalFactors(calibrationSettings.calFactor);
	loadDCoffsets(calibrationSettings.DCoffset);
	loadACoffsets(calibrationSettings.ACoffset);

	ads1256 = new Ads1256();
	ads1256->command(CMD_SYNC);
	ads1256->command(CMD_WAKEUP);
	vTaskDelay(100);
	ads1256->setDataRate(datarate);
	vTaskDelay(100);
	ads1256->setChannels(AN0, AN1);  // AN0 is common
	ads1256->setPGA(PGA2);
	PGA = PGA2;

	vTaskDelay(100);
	//ads1256->readAllRegisters(&ads1256RegsCpy);

	ads1256->command(CMD_SELFCAL);
	vTaskDelay(100);
	ads1256->command(CMD_SYNC);
	ads1256->command(CMD_WAKEUP);
	refAverager.setAverages(16);
}

ads1256PGA_t DMM::setPGA(ads1256PGA_t pga) {
	autoPGA = false;

	if (PGA < NRPGA)
		PGA = pga;
	ads1256->setPGA(PGA);
	return PGA;
}

ads1256PGA_t DMM::getPGA(void) {
	return PGA;
}

ads1256PGA_t DMM::changePGA(direction_t dir) {
	int p = (int) PGA;
	if (dir == DIRECTION_UP) {
		if (PGA < PGA64)
			p++;
	} else {
		if (PGA > PGA1)
			p--;
	}
	PGA = (ads1256PGA_t) p;
	ads1256->setPGA(PGA);
	ads1256->command(CMD_SELFCAL);
	return PGA;
}

void DMM::doSelfCal(void) {
	ads1256->command(CMD_SELFCAL);
}

int DMM::setRange(int newRange) {
	int range = DMMdescriptors::setRange(newRange);
	averager.clear();
	return range;
}
int DMM::changeRange(direction_t dir) {
	int range = DMMdescriptors::changeRange(dir);
	averager.clear();
	return range;
}

void DMM::setAutoRange(bool val) {
	autoRange = val;
}

bool DMM::getAutoRange(void) {
	return autoRange;
}

void* DMM::setAverages(uint32_t avgs) {
	averages = avgs;
	return averager.setAverages(avgs);
}

float DMM::setDataRate(ad1256DataRate_t dataRateIdx) {
	return ads1256->setDataRate(dataRateIdx);
}

void DMM::setAC(void) {
	AC = true;
	DMMdescriptors::setAC();
	setChannels(AN2, ANINCOM);
}

void DMM::setDC(void) {
	AC = false;
	DMMdescriptors::setDC();
	setChannels(AN0, AN1);
//	setChannels(AN1, AN0);
}
adsResult_t DMM::setChannels(ads1256Channel_t posChannel, ads1256Channel_t negChannel) {

	return ads1256->setChannels(posChannel, negChannel);
}

// uses the averaged value to calibrate
void DMM::calibrateCurrentRange(double calValue) {
	int32_t adcResult;
	double volt;
	int div = 1 << PGA;
	adcResult = averager.average();

	int range = getCurrentRange();
	if (AC) {
		adcResult -= getCurrentACoffset();

		//	if (range == RANGE_5V) // calibrate AC  . AC calfactor is a multiplier for all DC calfactors
		{
			volt = (5.0 * ((double) (adcResult) / 0x7FFFFF)) / (double) div;
			volt *= getCurrentDCGain();  // = previous set DC calfactor
			calibrationSettings.ACcalFactor = calValue / volt;
			for (int n = 0; n < NR_RANGES; n++)
				setACGain(calibrationSettings.ACcalFactor, n);
		}
		return;
	} else
		adcResult -= getCurrentDCoffset();

	switch (range) {

	default: {
		volt = (5.0 * ((double) (adcResult) / 0x7FFFFF)) / (double) div;
		double calFactor = calValue / volt;
		setCurrentGain(calFactor);
		calibrationSettings.calFactor[range] = calFactor;
		printf("\n%.*e\n", DBL_DIG - 1, volt);
		printf("\t%.*e\n", DBL_DIG - 1, calFactor);
		volt *= calFactor;
		printf("\t%.*e\n", DBL_DIG - 1, volt);
	}
	break;
	}
}

void DMM::writeSettings(void) {
	saveCalibrationSettings();
}


uint32_t avgTest = 10;

double DMM::read(void) {
	int32_t adcResult;
	double volt =0;
	static int autoDelay = 5;


	int div = 1 << PGA;

	ads1256->readData(&adcResult);
	realTimeValue = adcResult;
	rawValue = adcResult;

	// range is from  -8388608 to + 8388608 ( 0x800000 )
	if ( (adcResult <= -8388600) || (adcResult > 8388600)) { // overrange
		if (autoRange) {  // if autoranging and not in maximum range , suppress overrange display
			if (isMaxRange)
				volt =  OVERRANGEVALUE;
			else
				volt = OVERRANGEVALUE_NO_SHOW;
		}
		else
			volt =  OVERRANGEVALUE;
	}

	else {
		int range = getCurrentRange();
		printf("\n%d \t ", adcResult);
		if (AC)//  AC is referenced from 0V ( AGND ) instead of 2.5,
			adcResult = 0x400000-adcResult; // negate
		if (averages > 1) {
			averager.write(adcResult);
			adcResult = averager.average();
		}
		if (calcOffset) {
			calcOffset = false;
			if (AC) {
				setCurrentACoffset(adcResult);
				calibrationSettings.ACoffset[range] = adcResult;
			} else {
				setCurrentDCoffset(adcResult);
				calibrationSettings.DCoffset[range] = adcResult;
			}

			printf("%d  %d \n ", PGA, adcResult);
		}

		if (AC) {
			adcResult -= getCurrentACoffset();
			realTimeValue -= getCurrentACoffset();
		} else {
			adcResult -= getCurrentDCoffset();
			realTimeValue -= getCurrentDCoffset();
		}
		printf(" %d ", adcResult);

		actualVoltage = (5.0 * ((double) (adcResult) / 0x7FFFFF)) / (double) div;
		volt = actualVoltage * getCurrentDCGain();

		realTimeValue = (5.0 * ((double) (realTimeValue) / 0x7FFFFF)) / (double) div;
		realTimeValue *= getCurrentDCGain();
		if (AC) {
			volt *= getCurrentACGain();
			realTimeValue *= getCurrentACGain();
		}
	}

	if (autoRange) {
		if (adcResult < 0)
			adcResult = -adcResult;

		if (adcResult < (ADCFULLSCALE * 0.05)){
			if (autoDelay == 0) {
				changeRange(DIRECTION_DOWN);
				autoDelay = userSettings.conversionSpeed;
			}
			else
				autoDelay--;
		}
		else {
			if (adcResult > (ADCFULLSCALE * 0.8)) {
				if (autoDelay == 0) {
					changeRange(DIRECTION_UP);
						autoDelay = userSettings.conversionSpeed;
				}
				else
					autoDelay--;
			}
			else
				autoDelay = userSettings.conversionSpeed;
		}
	}

	return volt;
}

double DMM::getRealTimeValue(void) {
	return realTimeValue;
}

uint32_t DMM::getRawValue(void) {
	return rawValue;
}
double DMM::getActualVoltage(void){
	return actualVoltage;
}

void DMM::setOffset(void) {
	calcOffset = true;
}

// new descriptors received from host
int DMM::updateDescriptors(void) {
	char line[128];
	char *len;
	int num = 0;

	bool stop = false;
	uint32_t DCoffset, ACoffset;
	double DCgain, ACgain;

	FILE *f = fopen("/spiffs/descriptors.dmm", "r");
	if (f == NULL) {
		printf("Failed to open descriptors.dmm\n");
		return -1;
	} else {
		fgets(line, sizeof(line), f);
		line[strlen(line) - 1] = 0; // strip newline
		if (strcmp(descriptorHeader, line) == 0) {  // 	{"type","unit","range","DCoffset","ACoffset","DCgain","ACgain"};
			fgets(line, sizeof(line), f);  // skip header
			do {
				len = fgets(line, sizeof(line), f);

				if (len) {
					char *p = line;
					for (int n = 0; n < 3; n++) {
						p = strchr(p, ','); // skip 3 commas
						if (p == NULL) {
							stop = true;
							break;
						}
						p++;
					}
					if (!stop) {
						int n = sscanf(p, "%d,%d,%lf,%lf", &DCoffset, &ACoffset, &DCgain, &ACgain);
						if (n == 4) {
							calibrationSettings.DCoffset[num] = DCoffset;
							calibrationSettings.ACoffset[num] = ACoffset;
							calibrationSettings.calFactor[num] = DCgain;
							calibrationSettings.ACcalFactor = ACgain;
							n = setParameters(DCoffset, ACoffset, DCgain, ACgain, num++);
							if (n != 0)
								stop = true;
						} else
							stop = true;
					}
				} else
					stop = true;
			} while (!stop);
		} else
			printf("error format descriptors.dmm\n");
	}
	fclose(f);
	if (num == NR_RANGES) {
		writeSettings();
		printf("settings received OK\n");
		return 0;
	} else
		printf("Error in received settings\n");

	return -1;
}

DMM::~DMM() {
	// TODO Auto-generated destructor stub
}

