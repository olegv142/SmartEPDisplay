#pragma once

#include <msp430.h>
#include <stdint.h>

#define __MSP430_CPU__

#define MCLK_MHz 8
#define MCLK_Hz (MCLK_MHz*1000000ULL)

#ifdef __cplusplus
extern "C" {
#endif

void platform_init(void);

extern volatile unsigned long _platform_uptime;

static inline unsigned long platform_uptime_ms(void)
{
	unsigned long up;
	__istate_t st = __get_interrupt_state();
	__disable_interrupt();
	up = _platform_uptime;
	__set_interrupt_state(st);
	return up;
}

#define LPM_BITS LPM3_bits

static inline void platform_idle(void)
{
	__bis_SR_register(LPM_BITS + GIE); // Enter LPM3, enable interrupts
}

static inline void platform_idle_loop(void)
{
	for (;;) {
		platform_idle();
	}
}

#define SEC 1000UL

static void platform_delay_ms(unsigned long ms)
{
	unsigned long start = platform_uptime_ms();
	for (;;) {
		platform_idle();
		unsigned long elapsed = platform_uptime_ms() - start;
		if (elapsed > ms)
			break;
	}
}

#ifdef __cplusplus
}
#endif
