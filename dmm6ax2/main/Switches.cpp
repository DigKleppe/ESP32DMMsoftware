/*
 * ADG1414.cpp
 *
 *  Created on: Oct 4, 2020
 *      Author: dig
 *      drives the SPI analog switches and shunt select lines
 */

#include "esp_system.h"
#include "driver/gpio.h"
#include "include/Switches.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

Switches::Switches() {
	gpio_set_direction(CURRENT_100mA_PIN, GPIO_MODE_OUTPUT);
	gpio_set_direction(CURRENT_1A_PIN, GPIO_MODE_OUTPUT);
	gpio_set_direction(CURRENT_10A_PIN, GPIO_MODE_OUTPUT);
	gpio_set_direction(REED_RELAY_PIN, GPIO_MODE_OUTPUT);

	gpio_set_level(CURRENT_100mA_PIN, 0);  // shunt on, inverts
	gpio_set_level(CURRENT_1A_PIN, 0);
	gpio_set_level(CURRENT_10A_PIN, 0);

	gpio_set_level(REED_RELAY_PIN, 0);

	gpio_set_direction(ADG1414Clk_PIN, GPIO_MODE_OUTPUT);
	gpio_set_direction(ADG1414Din_PIN, GPIO_MODE_OUTPUT);
	gpio_set_direction(ADG1414Sync_PIN, GPIO_MODE_OUTPUT);

	gpio_set_level(ADG1414Clk_PIN, 1);
	gpio_set_level(ADG1414Din_PIN, 1);
	gpio_set_level(ADG1414Sync_PIN, 1);
}

void Switches::write(uint32_t val) {
	if (val & ESP_SW_RANGE_10A)
		gpio_set_level(CURRENT_10A_PIN,0);  // driver inverts
	else
		gpio_set_level(CURRENT_10A_PIN,1);

	if (val & ESP_SW_RANGE_1A)
		gpio_set_level(CURRENT_1A_PIN,0);
	else
		gpio_set_level(CURRENT_1A_PIN,1);

	if (val & ESP_SW_RANGE_100mA)
		gpio_set_level(CURRENT_100mA_PIN,0);
	else
		gpio_set_level(CURRENT_100mA_PIN,1);

	if ( val & ESP_SW_REED)
		gpio_set_level(REED_RELAY_PIN,1);
	else
		gpio_set_level(REED_RELAY_PIN,0);


	writeADG414(val);

}

// device has maximum clk of 50Mhz
void Switches::writeADG414(uint32_t val) {
	uint32_t mask = 1;

	gpio_set_level(ADG1414Sync_PIN, 0);
	for ( int n =0; n< 16; n++) {
		gpio_set_level(ADG1414Clk_PIN, 1); // verify 9ns min
		gpio_set_level(ADG1414Din_PIN, val & mask);
		mask <<= 1;
		vTaskDelay(1);
		gpio_set_level(ADG1414Clk_PIN, 0);
		vTaskDelay(1);
	}
	gpio_set_level(ADG1414Clk_PIN, 1);
	gpio_set_level(ADG1414Sync_PIN, 1);
}

