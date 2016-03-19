#pragma once

/*
 * Arduino compatibility header
 */

#include "platform.h"

typedef uint8_t byte;

#define delayMicroseconds(us) __delay_cycles((us)*MCLK_MHz)
#define delay(ms) platform_delay_ms(ms)

static inline unsigned long millis(void)
{
	return platform_uptime_ms();
}

#define LOW  0
#define HIGH 1

