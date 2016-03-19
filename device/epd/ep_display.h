#pragma once

#include "platform.h"

#define EPD_W 264
#define EPD_H 176
#define EPD_LINE_BYTES (EPD_W/8)
#define EPD_IMAGE_BYTES (EPD_LINE_BYTES*EPD_H)

#ifdef __cplusplus
extern "C" {
#endif

void ep_display_init(void);

// Show image. Takes bitmap with MSB of each byte on the right side.
// Return 0 on success, -1 on failure.
int ep_display_image(uint8_t const* data);

#ifdef __cplusplus
}
#endif
