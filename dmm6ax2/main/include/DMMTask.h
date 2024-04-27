
#include "settings.h"


typedef struct {
	char averegedValue[MAX_STRLEN+1];
	char momentaryValue[MAX_STRLEN+1];

} measValues_t;

extern measValues_t measValues;

extern volatile bool newCalValueReceived;
extern double calValue;


