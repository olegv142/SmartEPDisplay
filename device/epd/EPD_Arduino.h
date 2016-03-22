#pragma once

#include "platform.h"

static inline void digitalWrite(int pin, int level)
{
	if (level) {
		P2OUT |= pin;
	} else {
		P2OUT &= ~pin;
	}
}

static inline int digitalRead(int pin)
{
	return (P2IN & pin) != 0;
}

class SPI_EPD_Class {
public:
	void begin() { UCB0CTLW0 &= ~UCSWRST; }
	void end()   { UCB0CTLW0 |= UCSWRST; }
	byte transfer(byte data) { 
			UCB0TXBUF = data;
			while (UCB0STATW & UCBUSY) __no_operation();
			return UCB0RXBUF;
		}

	void setBitOrder(uint8_t) {}
	void setDataMode(uint8_t) {}
	void setClockDivider(uint8_t) {}
};

extern SPI_EPD_Class EPD_SPI;

#define SPI EPD_SPI

