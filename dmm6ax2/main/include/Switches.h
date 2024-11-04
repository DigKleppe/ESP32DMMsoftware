/*
 * Adg1414.h
 *
 *  Created on: Oct 4, 2020
 *      Author: dig
 */

#ifndef MAIN_INCLUDE_ADG1414_H_
#define MAIN_INCLUDE_ADG1414_H_

#include <stdint.h>

// defines outputs connected to serial switches
// U7 - U3 - U1

//#define UxA					1
//#define UxB					2
//#define UxC					4
//#define UxD					8
//#define UxE					16
//#define UxF					32
//#define UxG					64
//#define UxH					128

#define UxA						128
#define UxB						64
#define UxC						32
#define UxD						16
#define UxE						8
#define UxF						4
#define UxG						2
#define UxH						1

#define U1_SW_A_low				UxA
#define U1_SW_A_High			UxB
#define U1_SW_100uA				UxC
#define U1_SW_1mA				UxD
#define U1_SW_10mA				(UxE | UxF)
#define U1_SW_RANGE_GAIN1		UxG
#define U1_SW_RANGE_GAIN10		UxH

// U3
#define U3_SW_RANGE_2_5_5V		(UxA<<8)
#define U3_SW_RANGE_50V			(UxB<<8)
#define U3_SW_RANGE_500V		(UxC<<8)

#define U3_SW_RANGE_OHM1mA		(UxD<<8)
#define U3_SW_RANGE_OHM100uA	(UxE<<8)
#define U3_SW_RANGE_OHM10uA		(UxF<<8)
#define U3_SW_RANGE_OHM1uA		(UxG<<8)
#define U3_SW_RANGE_OHMS_ON		(UxH<<8)

// High current switched by ESP pins
#define ESP_SW_RANGE_100mA		(1<<24)
#define ESP_SW_RANGE_1A			(1<<25)
#define ESP_SW_RANGE_10A		(1<<26)
// reed relay switched by ESP pin
#define ESP_SW_REED				(1<<27)

// software SPI, hardware SPI for LCD
// this bus is temporary used, less noise then if the LCD signals where present at analog circuits.
#define ADG1414Clk_PIN		GPIO_NUM_0
#define ADG1414Din_PIN 		GPIO_NUM_27
#define ADG1414Sync_PIN 	GPIO_NUM_2

#define CURRENT_100mA_PIN	GPIO_NUM_33
#define CURRENT_1A_PIN		GPIO_NUM_25
#define CURRENT_10A_PIN		GPIO_NUM_26

#define REED_RELAY_PIN		GPIO_NUM_32

typedef enum {
	SW_250mV = (ESP_SW_REED | U3_SW_RANGE_2_5_5V | U1_SW_RANGE_GAIN10 | ESP_SW_RANGE_10A| U1_SW_10mA),
	SW_2500mV = (ESP_SW_REED | U3_SW_RANGE_2_5_5V | U1_SW_RANGE_GAIN1 | ESP_SW_RANGE_10A| U1_SW_10mA),
	SW_2_5V = (ESP_SW_REED | U3_SW_RANGE_2_5_5V | U1_SW_RANGE_GAIN1 | ESP_SW_RANGE_10A| U1_SW_10mA),
	SW_5V = (U3_SW_RANGE_2_5_5V | U1_SW_RANGE_GAIN1 | ESP_SW_RANGE_10A| U1_SW_10mA),
	SW_50V = (U3_SW_RANGE_50V | U1_SW_RANGE_GAIN1 | ESP_SW_RANGE_10A| U1_SW_10mA),
	SW_500V = (U3_SW_RANGE_500V | U1_SW_RANGE_GAIN1 | ESP_SW_RANGE_10A| U1_SW_10mA),
	SW_10uA = ( U1_SW_RANGE_GAIN1 | U1_SW_A_low),
	SW_100uA = (U1_SW_100uA | U1_SW_RANGE_GAIN1 | U1_SW_A_low),
	SW_1mA = (U1_SW_1mA | U1_SW_RANGE_GAIN1 | U1_SW_A_low),
	SW_10mA = ( U1_SW_10mA | U1_SW_RANGE_GAIN1 | U1_SW_A_low),
	SW_100mA = (ESP_SW_RANGE_100mA | U1_SW_RANGE_GAIN1 | U1_SW_A_High),//
	SW_1A = (ESP_SW_RANGE_1A | U1_SW_RANGE_GAIN1 | U1_SW_A_High),
	SW_10A = (ESP_SW_RANGE_10A | U1_SW_RANGE_GAIN1 | U1_SW_A_High),
	SW_2kOhm = (ESP_SW_REED | U3_SW_RANGE_OHM1mA | U3_SW_RANGE_OHMS_ON | U1_SW_RANGE_GAIN1 | ESP_SW_RANGE_10A),
	SW_20kOhm = (ESP_SW_REED | U3_SW_RANGE_OHM100uA | U3_SW_RANGE_OHMS_ON | U1_SW_RANGE_GAIN1 | ESP_SW_RANGE_10A),
	SW_200kOhm = (ESP_SW_REED | U3_SW_RANGE_OHM10uA | U3_SW_RANGE_OHMS_ON | U1_SW_RANGE_GAIN1 | ESP_SW_RANGE_10A),
	SW_2MOhm = (ESP_SW_REED | U3_SW_RANGE_OHM1uA | U3_SW_RANGE_OHMS_ON | U1_SW_RANGE_GAIN1 | ESP_SW_RANGE_10A),
	SW_20MOhm = (ESP_SW_REED | U3_SW_RANGE_OHMS_ON | U1_SW_RANGE_GAIN1 | ESP_SW_RANGE_10A),
	SW_TEMP = ESP_SW_RANGE_10A
} InputSwitch_t;



class Switches {
	public:
	Switches();
	void write (uint32_t val);
	private:
	void writeADG414(uint32_t val);
};


#endif /* MAIN_INCLUDE_ADG1414_H_ */
