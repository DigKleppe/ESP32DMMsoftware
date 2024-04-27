/*
 * mpr121.c
 *
 *  Created on: 11 jan 2020
 *      Author: D.Kleppe
 */

#include <stdint.h>
#include "mpr121.h"

#define WRITE_BIT I2C_MASTER_WRITE              /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ                /*!< I2C master read */
#define ACK_CHECK_EN 0x1                        /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0                       /*!< I2C master will not check ack from slave */

#define MPR_ADDRESS  		0x5A

#define TRESHOLD_TOUCH		0xB  // 0xA
#define TRESHOLD_RELEASE	0x6  // 0x5

#define STARTBASELINE		150  // speed up initializing

static uint8_t baseline[12] = {
STARTBASELINE, STARTBASELINE, STARTBASELINE, STARTBASELINE,
STARTBASELINE, STARTBASELINE, STARTBASELINE, STARTBASELINE,
STARTBASELINE, STARTBASELINE, STARTBASELINE, STARTBASELINE };

void MPR121write(uint8_t device_address, uint8_t register_address,
		uint8_t *data, uint8_t count);
void MPR121read(uint8_t device_address, uint8_t register_address, uint8_t *data,
		uint8_t count);

struct MI {
	uint8_t address;
	uint8_t value;
};

const struct MI mi[] = {

{ 0x80, 0x63 }, //soft reset
		{ ELE_CFG, 0x00 }, { GPIO_EN, 0 }, // enable gpio
		{ GPIO_DIR, 0 }, // pins as outputs
		{ GPIO_CTRL0, 0 }, // pins as high side open drain for led driving, 17 with pullup
		{ GPIO_CTRL1, 0 }, //

		// Section A - Controls filtering when data is > baseline.
		{ MHD_R, 0x01 }, //max half delta - the Max Half Delta determines the largest magnitude of variation to pass through the third level filter
		{ NHD_R, 0x01 }, //Noise Half Delta - the noise half determines the incremental change when non-noise drift is detected
		{ NCL_R, 0x00 }, //noise count limit - the noise count limit determines the number of samples consecutively greater than the Max Half Delta necessary before it can be determined that it is none-noise.
		{ FDL_R, 0x00 }, // 00 ! //Filter delay limit - the filter delay limit determines the rate of operation of the filter. A larger number makes it operate slower.

		// Section B - Controls filtering when data is < baseline.
		{ MHD_F, 0x01 }, //max half delta - the Max Half Delta determines the largest magnitude of variation to pass through the third level filter
		{ NHD_F, 0x01 }, //Noise Half Delta - the noise half determines the incremental change when non-noise drift is detected
		{ NCL_F, 0xFF }, //noise count limit - the noise count limit determines the number of samples consecutively greater than the Max Half Delta necessary before it can be determined that it is none-noise.
		{ FDL_F, 0x02 }, //Filter delay limit - the filter delay limit determines

		{ ELE0_T, TRESHOLD_TOUCH }, { ELE0_R, TRESHOLD_RELEASE },

		{ ELE1_T, TRESHOLD_TOUCH }, { ELE1_R, TRESHOLD_RELEASE },

		{ ELE2_T, TRESHOLD_TOUCH }, { ELE2_R, TRESHOLD_RELEASE },

		{ ELE3_T, TRESHOLD_TOUCH }, { ELE3_R, TRESHOLD_RELEASE },

		{ ELE4_T, TRESHOLD_TOUCH }, { ELE4_R, TRESHOLD_RELEASE },

		{ ELE5_T, TRESHOLD_TOUCH }, { ELE5_R, TRESHOLD_RELEASE },

		{ ELE6_T, TRESHOLD_TOUCH }, { ELE6_R, 0x5 },

		{ ELE7_T, TRESHOLD_TOUCH }, { ELE7_R, TRESHOLD_RELEASE },

		{ ELE8_T, TRESHOLD_TOUCH }, { ELE8_R, TRESHOLD_RELEASE },

		{ ELE9_T, TRESHOLD_TOUCH }, { ELE9_R, TRESHOLD_RELEASE },

		{ ELE10_T, TRESHOLD_TOUCH }, { ELE10_R, TRESHOLD_RELEASE },

		{ ELE11_T, TRESHOLD_TOUCH }, { ELE11_R, TRESHOLD_RELEASE },
		// Section D
		// Set the Filter Configuration
		// Set ESI2
		{ FIL_CFG, 0b00000100 }, //16ms default

		// Section E
		// Electrode Configuration
		// Set ELE_CFG to 0x00 to return to standby mode

		// Section F
		// Enable Auto Config and auto Reconfig
		{ ATO_CFG0, 0b00001111 }, //0b00001011
		{ ATO_CFGU, 0xC9 }, { ATO_CFGL, 0x82 }, // LSL = 0.65*USL = 0x82 @3.3V
		{ ATO_CFGT, 0xB5 }, // Target = 0.9*USL = 0xB5 @3.3V
		//	{ELE_CFG, 0x0C},
		{ DEBOUNCE_REG, 0x00 }, //enable debounce in MPR //0-7
		{ ELE_CFG, (3 << 6) + 12 } //  ECR ,  Enables 12 Electrodes
};

MPR121::MPR121(uint8_t addr, i2c_port_t i2c_num) {
	i2caddr = addr;
	i2cportNum = i2c_num;
	init();
}

esp_err_t MPR121::write( uint8_t register_address,	uint8_t *data, uint8_t byte_count) {
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (i2caddr << 1) | WRITE_BIT, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, register_address, ACK_CHECK_EN);
	i2c_master_write(cmd, data, byte_count, ACK_CHECK_EN);
	i2c_master_stop(cmd);
	esp_err_t ret = i2c_master_cmd_begin(i2cportNum, cmd,
			1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);
	return ret;
}


esp_err_t MPR121::read(uint8_t register_address,uint8_t *data, uint8_t byte_count) {
	esp_err_t ret;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (i2caddr << 1) | WRITE_BIT, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, register_address, ACK_CHECK_EN);

	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (i2caddr << 1) | READ_BIT, ACK_CHECK_EN);
	if (byte_count > 1) {
		i2c_master_read(cmd, data, byte_count - 1, I2C_MASTER_ACK);
	}
	i2c_master_read_byte(cmd, data + byte_count - 1, I2C_MASTER_NACK);
	i2c_master_stop(cmd);
	ret = i2c_master_cmd_begin(i2cportNum, cmd,	100 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);
	return ret;
}

uint8_t MPR121::init(void) {
	uint8_t value;
	uint8_t regAddress;
	esp_err_t err = ESP_OK;
	for (int n = 0; (n <= sizeof(mi) / sizeof(mi[0])) && (err == ESP_OK);
			n++) {
		value = mi[n].value;
		regAddress = mi[n].address;
		err |= write( regAddress, &value, 1); // fill registers
	}
	if (err == ESP_OK)
		isInitialized = true;

	return err;

}

uint16_t MPR121::readTouchBits(void) {
	if (!isInitialized)
		init();
	read( TOUCH_STATUS_REGISTER_ADDRESS, (uint8_t*) &rxBuf, 2);
	return rxBuf;
}

void MPR121::readFilteredData(uint8_t *p) {
	if (!isInitialized)
		init();

	read( FILTERED_DATA0_REGISTER_ADDRESS, p, 24);
	return;
}

void MPR121::readBaselineData(uint8_t *p) {
	if (!isInitialized)
		init();

	read( BASELINE_DATA0_REGISTER_ADDRESS, p, 12);
	return;
}

void MPR121::writedBaselineData(uint8_t *p) {
	write( BASELINE_DATA0_REGISTER_ADDRESS, p, 12);

}
