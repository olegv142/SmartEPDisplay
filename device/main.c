#include "platform.h"
#include "hal_LCD.h"
#include "ep_display.h"

#define LED BIT0

const uint8_t image[EPD_IMAGE_BYTES];

static void show_test_image(void)
{
	uint8_t* im = (uint8_t*)image;
	int i, j;
	for (i = 0; i < EPD_H; ++i) {
		for (j = 0; j < EPD_LINE_BYTES; ++j) {
			im[i * EPD_LINE_BYTES + j] = (i / 8 + j) & 1 ? ~0 : 0;
		}
	}
	ep_display_image(image);
}

int main(void)
{
	platform_init();

	// GPIO Setup
	P1DIR = LED; // For LED

	// RTC setup
	RTCCTL0_H = RTCKEY_H;	// Unlock RTC key protected registers
	RTCCTL0_L = RTCRDYIE;	// Real-time clock ready interrupt enable
	RTCCTL1 = RTCMODE;	// to trigger interrupt every second
	RTCCTL1 &= ~RTCHOLD;	// Start RTC calendar mode
	RTCCTL0_H = 0;		// Lock RTC key protected registers

	Init_LCD();
	displayText("HELLO");

	ep_display_init();

	show_test_image();

	for (;;) {
		__bis_SR_register(LPM3_bits + GIE); // Enter LPM3, enable interrupts
	}
}

// RTC Interrupt Service Routine
#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void)
{
	switch (__even_in_range(RTCIV, RTCIV_RT1PSIFG))
	{
	case RTCIV_NONE:      break;	// Vector  0:  No pending interrupt
	case RTCIV_RTCOFIFG:  break;	// Vector  2:  Oscillator fault
	case RTCIV_RTCRDYIFG:		// Vector  4:  RTC Ready
		P1OUT ^= LED;		// Toggle P1.0
		break;
	case RTCIV_RTCTEVIFG: break;	// Vector  6:  RTC interval timer
	case RTCIV_RTCAIFG:   break;	// Vector  8:  RTC user alarm
	case RTCIV_RT0PSIFG:  break;	// Vector  10:  RTC prescaler 0
	case RTCIV_RT1PSIFG:  break;	// Vector  12:  RTC prescaler 1
	default: break;
	}
}

