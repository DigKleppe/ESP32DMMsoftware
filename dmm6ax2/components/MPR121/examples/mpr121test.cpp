#include "../../MPR121/src/mpr121.h"

#include <Streaming.h>
#include <Wire.h>
#include "../../../../arduinoLib/arduinoCores/Arduino.h"

TwoWire wire(1);
MPR121 mpr121(&wire);

uint16_t baselines[12];
uint16_t values[12];

extern "C" void app_main() {
	uint16_t touchbits;
	Serial.begin(115200);
	Serial << "mpr121 test\n";

	wire.begin(21, 22, 400000);

	while (1) {
		// mpr121.readBaselineData((uint8_t *)  baselines);
		// mpr121.readFilteredData((uint8_t *) values);
		touchbits = mpr121.readTouch();
		Serial << touchbits << "\n";
		vTaskDelay(50);

	}
}
