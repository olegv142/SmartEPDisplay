#pragma once

#include "platform.h"

/*
 * Dummy Arduino SPI class implementation.
 * All flags ignored, initialization must be made somewhere else.
 */

#define SPI_CLOCK_DIV2   0
#define SPI_CLOCK_DIV4   0
#define SPI_CLOCK_DIV8   0
#define SPI_CLOCK_DIV16  0
#define SPI_CLOCK_DIV32  0
#define SPI_CLOCK_DIV64  0
#define SPI_CLOCK_DIV128 0

#define SPI_MODE0 0
#define SPI_MODE1 0
#define SPI_MODE2 0
#define SPI_MODE3 0

#define MSBFIRST 0

class SPIClass {
public:
	virtual void begin() {}
	virtual void end() {}
	virtual byte transfer(byte _data) = 0;
	void setBitOrder(uint8_t) {}
	void setDataMode(uint8_t) {}
	void setClockDivider(uint8_t) {}
};
