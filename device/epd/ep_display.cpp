#include "ep_display.h"
#include "EPD2.h"
#include "EPD_Arduino.h"

// P2 pins
#define EPD_PANEL_ON  BIT7
#define EPD_BORDER    BIT1
#define EPD_DISCHARGE BIT3
#define EPD_RESET     BIT6
#define EPD_BUSY      BIT2
#define EPD_CS        BIT5

#define EPD_OUT_BITS (EPD_PANEL_ON|EPD_BORDER|EPD_DISCHARGE|EPD_RESET|EPD_CS)
#define EPD_IN_BITS  (EPD_BUSY)
#define EPD_BITS     (EPD_OUT_BITS|EPD_IN_BITS)

// P1 UCB0 pins
#define EPD_MOSI BIT6
#define EPD_MISO BIT7
#define EPD_SCLK BIT4

SPI_EPD_Class EPD_SPI;

static EPD_Class EPD_Inst(EPD_2_7,
		  EPD_PANEL_ON,
		  EPD_BORDER,
		  EPD_DISCHARGE,
		  EPD_RESET,
		  EPD_BUSY,
		  EPD_CS);

#ifdef __cplusplus
extern "C" {
#endif

void ep_display_init(void)
{
	P2OUT &= ~EPD_BITS;
	P2REN &= ~EPD_BITS;
	P2DIR |=  EPD_OUT_BITS;
	P2DIR &= ~EPD_IN_BITS;
	P1SEL0 |= EPD_MOSI|EPD_MISO|EPD_SCLK;

	UCB0CTLW0 = UCSWRST;                          // **Put state machine in reset**
	UCB0CTLW0 |= UCMST | UCSYNC | UCCKPH | UCMSB; // 3-pin, 8-bit SPI master
                                                  // Clock phase high, MSB first
	UCB0CTLW0 |= UCSSEL__SMCLK;  // SMCLK
	UCB0BRW = 0;                 // No division
}

int ep_display_image(uint8_t const* data)
{
	EPD_Inst.begin();
	if (!EPD_Inst.error()) {
		EPD_Inst.image(data);
		EPD_Inst.end();
		return 0;
	}
	return -1;
}

#ifdef __cplusplus
}
#endif
