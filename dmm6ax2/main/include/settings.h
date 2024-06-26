/*
 * settings.h
 *
 *  Created on: Nov 30, 2017
 *      Author: dig
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <stdbool.h>
#include "esp_system.h"
#include <time.h>
#include <sys/time.h>
#include "DMMdescriptors.h"

typedef enum { FLT, STR, INT , DESCR , CALVAL} varType_t;
#define MAX_STRLEN 32

#define USERSETTINGS_CHECKSTR 	"DMM"
#define CALCHECKSTR  "cal123"

typedef struct {
	int precisionOffset;
	int averages;
	int conversionSpeed;
	int udpPort;
	char moduleName[MAX_STRLEN+1];
	char checkstr[MAX_STRLEN+1];
}userSettings_t;

typedef struct {
	int32_t DCoffset[NR_RANGES];
	int32_t ACoffset[NR_RANGES];
	int32_t ohmsOffset[NR_RANGES];
	double calFactor[NR_RANGES];
	double ACcalFactor;
	char checkstr[MAX_STRLEN+1];
} calibrationSettings_t;

typedef struct {
	varType_t varType;
	int size;
	void * pValue;
	int minValue;
	int maxValue;
} settingsDescr_t;


typedef enum {SETTINGS_PREC, SETTINGS_AVG, SETTINGS_SPEED } settingsID_t;
extern settingsDescr_t settingsDescr[];

esp_err_t saveUserSettings( void);

esp_err_t saveSettings(void);  // wifi and usersettings
esp_err_t loadSettings(void);

esp_err_t saveCalibrationSettings( void);
esp_err_t loadCalibrationSettings( void);

extern userSettings_t 			userSettings;
extern calibrationSettings_t 	calibrationSettings;



#endif /* SETTINGS_H_ */
