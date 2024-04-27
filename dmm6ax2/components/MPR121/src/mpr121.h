/*
 * mpr121.h
 *
 *  Created on: 18 mrt. 2014
 *      Author: h.lepoeter
 */

#ifndef MPR121_H_
#define MPR121_H_

#include <stdint.h>
#include "driver/i2c.h"


#define MPR_ADDRESS  		0x5A
//
//#include <Arduino.h>
//#include <Wire.h>
// MPR121 Register Defines

#define TOUCH_STATUS_REGISTER_ADDRESS 0
#define OUT_OF_RANGE_STATUS_REGISTER_ADDRESS 2
#define FILTERED_DATA0_REGISTER_ADDRESS 4
#define BASELINE_DATA0_REGISTER_ADDRESS 0x1E

#define MHD_R 0x2B
#define NHD_R 0x2C
#define NCL_R 0x2D
#define FDL_R 0x2E
#define MHD_F 0x2F
#define NHD_F 0x30
#define NCL_F 0x31
#define FDL_F 0x32
#define ELE0_T 0x41
#define ELE0_R 0x42
#define ELE1_T 0x43
#define ELE1_R 0x44
#define ELE2_T 0x45
#define ELE2_R 0x46
#define ELE3_T 0x47
#define ELE3_R 0x48
#define ELE4_T 0x49
#define ELE4_R 0x4A
#define ELE5_T 0x4B
#define ELE5_R 0x4C
#define ELE6_T 0x4D
#define ELE6_R 0x4E
#define ELE7_T 0x4F
#define ELE7_R 0x50
#define ELE8_T 0x51
#define ELE8_R 0x52
#define ELE9_T 0x53
#define ELE9_R 0x54
#define ELE10_T 0x55
#define ELE10_R 0x56
#define ELE11_T 0x57
#define ELE11_R 0x58
#define FIL_CFG 0x5D
#define ELE_CFG 0x5E
#define DEBOUNCE_REG	0x5B
#define GPIO_CTRL0 0x73
#define GPIO_CTRL1 0x74
#define GPIO_DATA 0x75
#define GPIO_DIR 0x76
#define GPIO_EN 0x77
#define GPIO_SET 0x78
#define GPIO_CLEAR 0x79
#define GPIO_TOGGLE 0x7A
#define ATO_CFG0 0x7B
#define ATO_CFGU 0x7D
#define ATO_CFGL 0x7E
#define ATO_CFGT 0x7F

class MPR121 {
public:
	// MPR121(TwoWire *p);
	MPR121(uint8_t addr, i2c_port_t i2c_num);
	uint16_t readTouchBits(void);
	void readFilteredData(uint8_t *p); // 12 words
	void readBaselineData(uint8_t *p); // 12 bytes
	void writedBaselineData(uint8_t *p); // 12 bytes
	uint8_t init(void);

private:
	uint8_t i2caddr = 0;
	i2c_port_t i2cportNum = 0;
	esp_err_t err = ESP_OK;

	esp_err_t write( uint8_t register_address,uint8_t *data, uint8_t byte_count);
	esp_err_t read( uint8_t register_address,	uint8_t *data, uint8_t byte_count);

	bool isInitialized = false;
	uint16_t rxBuf;

};

#endif /* MPR121_H_ */
