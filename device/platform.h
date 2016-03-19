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

#ifdef __cplusplus
}
#endif
