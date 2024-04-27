/*
 * ADS1256.h
 *
 *  Created on: Nov 14, 2019
 *      Author: dig
 */

#ifndef MAIN_INCLUDE_ADS1256_H_
#define MAIN_INCLUDE_ADS1256_H_

extern "C" {
#include 	"driver/gpio.h"
}

#define ADS1256CS_PIN		GPIO_NUM_4	//cs pin. rest of SPI pins in LCD.c
#define ADS1256nDRDY_PIN	GPIO_NUM_16	//nDRDY pin


typedef enum {
	CMD_WAKEUP = 0,		//Completes SYNC and Exits Standby Mode
	CMD_RDATA = 1,		//Read Data
	CMD_RDATAC = 3,		//Read Data Continuously
	CMD_SDATAC = 0xF,	//Stop Read Data Continuously
	CMD_RREG = 0x10,	//Read from REG rrrr  2nd comman
	CMD_WREG = 0x50,	//Write to REG rrrr   2nd comman
	CMD_SELFCAL = 0xF0,	//Offset and Gain Self-Calibrati
	CMD_SELFOCAL = 0xF1,//Offset Self-Calibration
	CMD_SELFGCAL = 0xF2,//Gain Self-Calibration
	CMD_SYSOCAL = 0xF3,	//System Offset Calibration
	CMD_SYSGCAL = 0xF4,	//System Gain Calibration
	CMD_SYNC = 0xFC,	//Synchronize the A/D Conversion
	CMD_STANDBY = 0xFD,	//Begin Standby Mode
	CMD_RESET = 0xFE,	//Reset to Power-Up Values
} command_t;


//#define CMD_WAKEUP		0xFF	//Completes SYNC and Exits Standby Mode

typedef struct {
	union {
		uint8_t reg;
		struct {
			uint8_t DRDY :1;  // bit 0
			uint8_t BUFEN :1;
			uint8_t ACAL :1;
			uint8_t ORDER :1;
			uint8_t ID :4;
		} bit;
	};
} statusReg_t;

typedef struct {
	union {
		uint8_t reg;
		struct {
			uint8_t NSEL :4;  // bit 0
			uint8_t PSEL :4;
		} bit;
	};
} muxReg_t;

typedef struct {
	union {
		uint8_t reg;
		struct {
			uint8_t PGA :3;  // bit 0
			uint8_t SDCSL :2;
			uint8_t CLK :2;
		} bit;
	};
} adconReg_t;

typedef struct {
	union {
		uint8_t reg;
		struct {
			uint8_t DR :8;  // bit 0
		} bit;
	};
} drateReg_t;

typedef struct {
	union {
		uint8_t reg;
		struct {
			uint8_t DIO :4;  // bit 0
			uint8_t DIR :4;
		} bit;
	};
} ioReg_t;

typedef struct {
	uint8_t ofc[3];
} ofcReg_t;

typedef struct {
	uint8_t fsc[3];
} fscReg_t;

#pragma pack(1)
struct strAds1256Regs {
	statusReg_t STATUS;
	muxReg_t MUX;
	adconReg_t ADCON;
	drateReg_t DRATE;
	ioReg_t IO;
	ofcReg_t OFC;
	fscReg_t FSC;
};
#pragma pack()

typedef struct strAds1256Regs ads1256Regs_t;

typedef struct {
	uint8_t command1;
	uint8_t command2;
	uint8_t data[sizeof(ads1256Regs_t)];
}ads1256SPIbuf_t;


typedef enum {ADSRESULT_OK=0, ADSRESULT_BUSY= 400, ADSRESULT_SPI_ERR} adsResult_t;
typedef enum {STATUS_REG , MUX_REG, ADCON_REG, DRATE_REG, GPIOCONTROL_REG } adsRegAddr_t;

typedef enum { BUFFER_DISABLED , BUFFER_ENABLED } bufferEnabled_t;
typedef enum { AUTOCAL_DISABLED , AUTOCAL_ENABLED } autocalEnabled_t;
typedef enum { ORDER_MSB_FIRST , ORDER_LSBFIRST } order_t;

typedef enum {
	AN0, AN1, AN2, AN3, AN4, AN5, AN6, AN7, ANINCOM, NOCHANNEL
} ads1256Channel_t;

typedef enum {
	CLK_OFF, CLKIN, CLKIN_DIV2, CLKIN_DIV4
} ads1256ClkOut_t;
typedef enum {
	CURR_OFF, _05uA, _2uA, _10uA
} ads1256SensorDetCurrenSource_t;
typedef enum {
	PGA1, PGA2, PGA4, PGA8, PGA16, PGA32, PGA64, NRPGA
} ads1256PGA_t;

typedef enum {
	_2_5SPS,
	_5SPS,
	_10SPS,
	_15SPS,
	_25SPS,
	_30SPS,
	_50SPS,
	_60SPS,
	_100SPS,
	_500SPS,
	_1kSPS,
	_2kSPS,
	_3_75kSPS,
	_7_5kSPS,
	_15kSPS,
	_30kSPS,
} ad1256DataRate_t;

typedef struct {
	uint8_t datarateCode;
	float dataRate;
} strDataRate_t;



typedef enum {
	input = 1, output = 0
} ads1256GPIO_t;


class Ads1256 {
public:
	Ads1256();
	void initialize(void);
	adsResult_t readAllRegisters(ads1256Regs_t *ads1256Regs);
	adsResult_t writeAllRegisters(ads1256Regs_t *ads1256Regs);
	adsResult_t setChannels(ads1256Channel_t posChannel,
			ads1256Channel_t negChannel);
	adsResult_t setStatusRegister( bufferEnabled_t bufferEnabled, autocalEnabled_t autoCal, order_t order );
	adsResult_t setPGA(ads1256PGA_t pga);
	int getPGA (void );
	float setDataRate(ad1256DataRate_t dataRateIdx);
	adsResult_t setGPIOtype(ads1256GPIO_t D0, ads1256GPIO_t D1,
			ads1256GPIO_t D2, ads1256GPIO_t D3);
	adsResult_t setGPIO(uint8_t value);
	adsResult_t readGPIO(uint8_t*);
	adsResult_t readData(int32_t*);
	adsResult_t command(command_t command );
	adsResult_t dataReady (void );

	bool autoPGA = false;

private:
	ads1256Regs_t ads1256Regs;
	ads1256SPIbuf_t adsSPIbuf;
	adsResult_t initSPI (void);
	adsResult_t transfer (ads1256SPIbuf_t * spiBuf);
	//spi_lobo_device_handle_t ads1256spi = NULL;
	esp_err_t IRAM_ATTR ads1256Select(void);
	esp_err_t IRAM_ATTR ads1256DeSelect(void);
	ads1256PGA_t PGA= PGA2;
	float dataRate = 0;

};

#endif /* MAIN_INCLUDE_ADS1256_C_ */
