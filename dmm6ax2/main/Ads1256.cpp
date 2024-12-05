/*
 * ads1256.c
 *
 *  Created on: Nov 14, 2019
 *      Author: dig
 */
 
extern "C" {
#include <errno.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
}

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "include/Ads1256.h"

extern "C" void disp_wait_for_pending_transactions(void);
extern SemaphoreHandle_t xGuiSemaphore;  // used by lvgl, shares the same SPI bus

void delayT6(void);
#define SPI_BUS HSPI_HOST

static const strDataRate_t dataRateTable[] = {
	{ 0b00000011, 2.5 },
	{ 0b00010011, 5 },
	{ 0B00100011, 10 },
	{ 0B00110011, 15 },
	{ 0B01000011, 25 },
	{ 0B01010011, 30 },
	{ 0B01100011, 50 },
	{ 0B01110010, 60 },
	{ 0B10000010, 100 },
	{ 0B10010010, 500 },
	{ 0B10100001, 1000 },
	{ 0B10110000, 2000 },
	{ 0B11000000, 37500 },
	{ 0B11010000, 7500 },
	{ 0B11100000, 15000 },
	{ 0B11110000, 30000 }
};


void swap32(uint8_t *p) {
	typedef union {
		uint32_t u;
		uint8_t b[4];
	} conv_t;
	conv_t in;
	conv_t *out;
	in.u = *(uint32_t*) p;
	out = (conv_t*) p;
	out->b[0] = in.b[3];
	out->b[1] = in.b[2];
	out->b[2] = in.b[1];
	out->b[3] = in.b[0];
}


Ads1256::Ads1256() {
	initialize();
	vTaskDelay(5);
	command(CMD_RESET);
	vTaskDelay(5);

	setStatusRegister(BUFFER_DISABLED, AUTOCAL_ENABLED, ORDER_MSB_FIRST);

}

uint8_t zerosBuffer[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void Ads1256::initialize(void) {
	initSPI();
	gpio_set_direction(ADS1256nDRDY_PIN, GPIO_MODE_INPUT);
	gpio_set_pull_mode(ADS1256nDRDY_PIN,GPIO_PULLUP_ONLY);
	gpio_set_level(ADS1256CS_PIN, 1);
	gpio_set_direction(ADS1256CS_PIN, GPIO_MODE_OUTPUT);
	memset((void*) &ads1256Regs, 0, sizeof(ads1256Regs_t));
}

adsResult_t Ads1256::dataReady(void) {
	if (gpio_get_level(ADS1256nDRDY_PIN))
		return ADSRESULT_BUSY;
	else
		return ADSRESULT_OK;
}

adsResult_t Ads1256::writeAllRegisters(ads1256Regs_t *ads1256Regs) {
	adsResult_t res;
	adsSPIbuf.command1 = CMD_WREG; //  write from address 0
	adsSPIbuf.command2 = sizeof(ads1256Regs_t); // nr registers to write
	memcpy(&adsSPIbuf.data, ads1256Regs, sizeof(ads1256Regs_t)); // copy registerdata
	res = transfer(&adsSPIbuf);
	return res;
}

adsResult_t Ads1256::readAllRegisters(ads1256Regs_t *ads1256Regs) {
	adsResult_t res;
	adsSPIbuf.command1 = CMD_RREG; // read from address 0
	adsSPIbuf.command2 = sizeof(ads1256Regs_t); // nr registers to read
	res = transfer(&adsSPIbuf);
	if (res == ADSRESULT_OK)
		memcpy(ads1256Regs, &adsSPIbuf.data, sizeof(ads1256Regs_t));
	// todo rearrange
	return res;
}

adsResult_t Ads1256::command(command_t command) {
	adsResult_t res;
	adsSPIbuf.command1 = command;
	res = transfer(&adsSPIbuf);
	return res;
}

adsResult_t Ads1256::setStatusRegister(bufferEnabled_t bufferEnabled,
		autocalEnabled_t autoCal, order_t order) {
	adsResult_t res;
	adsSPIbuf.command1 = CMD_WREG | STATUS_REG;
	adsSPIbuf.command2 = 1;
	adsSPIbuf.data[0] = (bufferEnabled << 1) | (autoCal << 2) | (order << 3);
	res = transfer(&adsSPIbuf);
	return res;
}

adsResult_t Ads1256::setChannels(ads1256Channel_t posChannel,
		ads1256Channel_t negChannel) {
	adsResult_t res;
	adsSPIbuf.command1 = CMD_WREG | MUX_REG;
	adsSPIbuf.command2 = 1;
	adsSPIbuf.data[0] = (posChannel << 4) + negChannel;
	res = transfer(&adsSPIbuf);
	return res;
}

adsResult_t Ads1256::setPGA(ads1256PGA_t pga) {
	adsResult_t res;
	ads1256Regs.ADCON.bit.PGA = pga;
	adsSPIbuf.command1 = CMD_WREG | ADCON_REG;
	adsSPIbuf.command2 = 1;
	adsSPIbuf.data[0] = ads1256Regs.ADCON.reg;
	res = transfer(&adsSPIbuf);
	return res;
}

int Ads1256::getPGA(void) {
	return ads1256Regs.ADCON.bit.PGA;
}

float Ads1256::setDataRate ( ad1256DataRate_t dataRateIdx) {
	adsResult_t res;
	ads1256Regs.DRATE.reg = dataRateTable[dataRateIdx].datarateCode;
	dataRate = dataRateTable[dataRateIdx].dataRate;

	adsSPIbuf.command1 = CMD_WREG | DRATE_REG;
	adsSPIbuf.command2 = 1;
	adsSPIbuf.data[0] = ads1256Regs.DRATE.reg;
	res = transfer(&adsSPIbuf);
	if (res == ADSRESULT_OK)
		return dataRate;
	else
		return -1;
}

adsResult_t Ads1256::setGPIOtype(ads1256GPIO_t D0, ads1256GPIO_t D1,
		ads1256GPIO_t D2, ads1256GPIO_t D3) {
	adsResult_t res;
	ads1256Regs.IO.bit.DIR = D0 + (D1 << 1) + (D2 << 2) + (D3 << 3);
	adsSPIbuf.command1 = CMD_WREG | GPIOCONTROL_REG;
	adsSPIbuf.command2 = 1;
	adsSPIbuf.data[0] = ads1256Regs.IO.reg;
	res = transfer(&adsSPIbuf);
	return res;
}

adsResult_t Ads1256::setGPIO(uint8_t value) {
	adsResult_t res;
	ads1256Regs.IO.bit.DIO = value & 0x0F;
	adsSPIbuf.command1 = CMD_WREG | GPIOCONTROL_REG;
	adsSPIbuf.command2 = 1;
	adsSPIbuf.data[0] = ads1256Regs.IO.reg;
	res = transfer(&adsSPIbuf);
	return res;
}

adsResult_t Ads1256::readGPIO(uint8_t *p) {
	adsResult_t res;
	adsSPIbuf.command1 = CMD_RREG | GPIOCONTROL_REG;
	adsSPIbuf.command2 = 1;
	res = transfer(&adsSPIbuf);
	*p = adsSPIbuf.data[0] & 0x0F;
	return res;
}
adsResult_t Ads1256::readData(int32_t *buffer) {
	adsResult_t res;
	int32_t adcResult = 0;
	res = command(CMD_RDATA);
	if (res == ADSRESULT_OK) {
		memcpy((void*) &adcResult, &adsSPIbuf.data, sizeof(int32_t));
		adcResult >>= 8;
		*buffer = adcResult;
	}

	return res;
}

/*
 *  SPI part

 *  spi bus is initialized in the LCD driver,
 *
 */
static spi_device_handle_t ads1256spi;
const spi_device_interface_config_t ads1256devcfg = { .command_bits = 0, ///< Amount of bits in command phase (0-16)
		.address_bits = 0,           ///< Amount of bits in address phase (0-64)
		.dummy_bits = 0, ///< Amount of dummy bits to insert between address and data phase
		.mode = 1,                       ///< SPI mode (0-3)
		.duty_cycle_pos = 128, ///< Duty cycle of positive clock, in 1/256th increments (128 = 50%/50% duty). Setting this to 0 (=not setting it) is equivalent to setting this to 128.
		.cs_ena_pretrans = 1, ///<  Amount of SPI bit-cycles the cs should be activated before the transmission (0-16). This only works on half-duplex transactions.
		.cs_ena_posttrans = 0, ///< Amount of SPI bit-cycles the cs should stay active after the transmission (0-16)
		.clock_speed_hz = (7680000 / 4), ///< Clock speed, in Hz Max 2.5 Mhz
		.input_delay_ns = 50,
		.spics_io_num = -1, ///< CS GPIO pin for this device, handled by hardware; set to -1 if not used
		.flags = 0,                    ///< Bitwise OR of LB_SPI_DEVICE_* flags
		.queue_size= 10,
		.pre_cb = NULL, ///< Callback to be called before a transmission is started.
		.post_cb = NULL ///< Callback to be called after a transmission has completed.
};

//-------------------------------
esp_err_t IRAM_ATTR Ads1256::ads1256Select() {

	// wait until lvgl semaphore comes available
	if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY)) {
		disp_wait_for_pending_transactions();
		gpio_set_level(ADS1256CS_PIN, 0);
	}
	return ESP_OK;
}

//---------------------------------
esp_err_t IRAM_ATTR Ads1256::ads1256DeSelect() {

	gpio_set_level(ADS1256CS_PIN, 1);
	xSemaphoreGive (xGuiSemaphore);
	return ESP_OK;
}

adsResult_t Ads1256::initSPI(void) {

    esp_err_t res=spi_bus_add_device(SPI_BUS, &ads1256devcfg, &ads1256spi);
	assert(res==ESP_OK);
	printf("SPI: ADS1256 added to spi bus (%d)\r\n", SPI_BUS);
	return (adsResult_t) res;
}

adsResult_t Ads1256::transfer(ads1256SPIbuf_t *spiBuf) {
	esp_err_t res = ESP_OK;
	bool handled = false;

	spi_transaction_t transaction;
	memset((uint8_t*) &transaction, 0, sizeof(transaction));

	transaction.tx_data[0] = spiBuf->command1; // command + startregister
	transaction.tx_data[1] = spiBuf->command2 - 1; // nr bytes to read or write
	transaction.flags = SPI_TRANS_USE_TXDATA;
	ads1256Select();

	if (res == ESP_OK) {
		switch ((spiBuf->command1 & 0xF0)) {
		case CMD_RREG:   // read registers(s)
			handled = true;
			transaction.length = 16;	// send 2 bytes
			res = spi_device_transmit(ads1256spi, &transaction);
			assert(res==ESP_OK);
			//	delayT6(); not neededed , already delay of 6.5us
			transaction.flags = 0;
			transaction.tx_buffer = zerosBuffer;
			transaction.length = spiBuf->command2 * 8;	// send 0 bytes
			transaction.rxlength = spiBuf->command2 * 8;
			transaction.flags = 0; // store in spiBuf
			transaction.rx_buffer = (void*) &spiBuf->data;
			res = spi_device_transmit(ads1256spi, &transaction);
			assert(res==ESP_OK);
			break;

		case CMD_WREG:  // write register(s)
			handled = true;
			transaction.length = 16;	// send 2 bytes
			res = spi_device_transmit(ads1256spi, &transaction);
			assert(res==ESP_OK);
			//	delayT6();

			transaction.length = 8 * spiBuf->command2;// send nr bytes in command2
			transaction.rxlength = 0;
			transaction.flags = 0; // store in spiBuf
			transaction.tx_buffer = (const void*) &spiBuf->data; // read from data
			res = spi_device_transmit(ads1256spi, &transaction);
			assert(res==ESP_OK);
			break;
		}

		if (!handled) {
			switch (spiBuf->command1) {
			case CMD_RDATA:
				if (gpio_get_level(ADS1256nDRDY_PIN) == 0) {
					transaction.length = 8;	// send 1 byte
					res = spi_device_transmit(ads1256spi, &transaction);
					assert(res==ESP_OK);
					//	delayT6();
					transaction.length = 24;
					transaction.rxlength = 24;  // read 3 bytes
					transaction.tx_buffer = zerosBuffer;
					transaction.flags = SPI_TRANS_USE_RXDATA; // store in rxdata
					res = spi_device_transmit(ads1256spi, &transaction);
					spiBuf->data[0] = 0;
					spiBuf->data[3] = transaction.rx_data[0];  // swap
					spiBuf->data[2] = transaction.rx_data[1];
					spiBuf->data[1] = transaction.rx_data[2];  // contains sign

					assert(res==ESP_OK);
				} else
					res = ADSRESULT_BUSY;
				break;

			default:  // one byte commands
				transaction.length = 8;	// send 1 byte
				transaction.flags = SPI_TRANS_USE_TXDATA;
				res = spi_device_transmit(ads1256spi, &transaction);
				assert(res==ESP_OK);
				break;
			}
		}
	}
	ads1256DeSelect();
	return (adsResult_t) res;
}

// 50 clks of 7.68MHz = 6.5us
// measured with scope @240MHz
#pragma GCC optimize ("O0")

void delayT6(void) {
	uint32_t x = 10;
	while (x--)
		;

}
