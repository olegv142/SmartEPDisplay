#include "ep_display.h"
#include "Arduino.h"
#include "SPI.h"
#include "EPD2.h"

#define EPD_PANEL_ON  IO_PIN(P2, 7)
#define EPD_BORDER    IO_PIN(P2, 1)
#define EPD_DISCHARGE IO_PIN(P2, 3)
#define EPD_RESET     IO_PIN(P2, 6)
#define EPD_BUSY      IO_PIN(P2, 2)
#define EPD_CS        IO_PIN(P2, 5)

#define EPD_OUT_BITS (uint8_t)(IO_BIT(EPD_PANEL_ON)|IO_BIT(EPD_BORDER)|IO_BIT(EPD_DISCHARGE)|IO_BIT(EPD_RESET)|IO_BIT(EPD_CS))
#define EPD_IN_BITS  (uint8_t)(IO_BIT(EPD_BUSY))
#define EPD_BITS     (uint8_t)(EPD_OUT_BITS|EPD_IN_BITS)

// P1 UCB0 pins
#define EPD_MOSI BIT6
#define EPD_MISO BIT7
#define EPD_SCLK BIT4

class EPD_SPI_Class : public SPIClass {
public:
	virtual void begin() { UCB0CTLW0 &= ~UCSWRST; }
	virtual void end()   { UCB0CTLW0 |= UCSWRST; }
	virtual byte transfer(byte data) { 
			UCB0TXBUF = data;
			while (UCB0STATW & UCBUSY) __no_operation();
			return UCB0RXBUF;
		}
};

static EPD_SPI_Class EPD_SPI_Inst;
SPIClass* EPD_SPI = &EPD_SPI_Inst;

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
