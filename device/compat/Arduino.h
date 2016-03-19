#pragma once

/*
 * Arduino compatibility header
 */

#include "platform.h"

typedef uint8_t byte;

#define delayMicroseconds(us) __delay_cycles((us)*MCLK_MHz)
#define delay(ms) delayMicroseconds((ms)*1000ULL)

static inline unsigned long millis(void)
{
	return platform_uptime_ms();
}

#define LOW  0
#define HIGH 1

enum {
	P1, P2, P3, P4, P5, P6, P7, P8, P9, P10
};

#define IO_PIN(port, pin) (((port) << 4) | pin)
#define IO_PORT(pin) ((pin) >> 4)
#define IO_BIT(pin) (1 << ((pin) & 0xf))

extern volatile uint8_t* _gp_out_port_map[];
extern volatile uint8_t const* _gp_in_port_map[];

static inline void digitalWrite(int pin, int level)
{
	if (level) {
		*_gp_out_port_map[IO_PORT(pin)] |= IO_BIT(pin);
	} else {
		*_gp_out_port_map[IO_PORT(pin)] &= ~IO_BIT(pin);
	}
}

static inline int digitalRead(int pin)
{
	return (*_gp_in_port_map[IO_PORT(pin)] & IO_BIT(pin)) != 0;
}
