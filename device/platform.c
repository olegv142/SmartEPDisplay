#include "platform.h"

volatile unsigned long _platform_uptime;

void platform_init(void)
{
	WDTCTL = WDT_ADLY_16;  // WDT 16ms, ACLK, interval timer

	// To avoid floating
	P1OUT = 0;
	P2OUT = 0;
	P3OUT = 0;
	P4OUT = 0;
	P5OUT = 0;
	P6OUT = 0;
	P7OUT = 0;
	P8OUT = 0;
	P9OUT = 0;
	P10OUT = 0;
	PJOUT = 0;

	P1REN = ~0;
	P2REN = ~0;
	P3REN = ~0;
	P4REN = ~0;
	P5REN = ~0;
	P6REN = ~0;
	P7REN = ~0;
	P8REN = ~0;
	P9REN = ~0;
	P10REN = ~0;
	PJREN = ~0;

	// Set up XT1
	PJSEL0 = BIT4 | BIT5;                     // For XT1

	// Clock System Setup
	CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
	CSCTL1 = DCOFSEL_6;                       // Set DCO to 8MHz
	CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
	CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
	CSCTL4 &= ~LFXTOFF;

	// Disable the GPIO power-on default high-impedance mode to activate
	// previously configured port settings
	PM5CTL0 &= ~LOCKLPM5;

	SFRIE1 = OFIE | WDTIE;                   // Enable osc fault and WDT interrupts

	__enable_interrupt();
}

// Oscillator fault handler
#pragma vector=UNMI_VECTOR
__interrupt void UNMI_ISR(void)
{
	do
	{
		CSCTL5 &= ~LFXTOFFG;  // Clear XT1 fault flag
		SFRIFG1 &= ~OFIFG;
		__delay_cycles(100);  // Time for flag to set
	} while (SFRIFG1 & OFIFG);    // Test oscillator fault flag
}

#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	_platform_uptime += 16;
}
