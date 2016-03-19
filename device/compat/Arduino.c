#pragma once

/*
 * Arduino compatibility stuff
 */

#include "Arduino.h"

volatile uint8_t* _gp_out_port_map[] = 
{
	[P1] = &P1OUT,
	[P2] = &P2OUT,
	[P3] = &P3OUT,
	[P4] = &P4OUT,
	[P5] = &P5OUT,
	[P6] = &P6OUT,
	[P7] = &P7OUT,
	[P8] = &P8OUT,
	[P9] = &P9OUT,
	[P10] = &P10OUT,
};

volatile uint8_t const* _gp_in_port_map[] = 
{
	[P1] = &P1IN,
	[P2] = &P2IN,
	[P3] = &P3IN,
	[P4] = &P4IN,
	[P5] = &P5IN,
	[P6] = &P6IN,
	[P7] = &P7IN,
	[P8] = &P8IN,
	[P9] = &P9IN,
	[P10] = &P10IN,
};
