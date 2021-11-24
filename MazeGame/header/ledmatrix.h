/*
ledmatrix7219d88 0x01

copyright (c) Davide Gironi, 2013

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/


#ifndef LEDMATRIX7219D88_H_
#define LEDMATRIX7219D88_H_

#include "max7219.h"

#define LEDMATRIX7219D88_MAXLEDMATRIX MAX7219_ICNUMBER //number of led matrix connected

//functions
extern void ld_resetmatrix(uint8_t ledmatrix);
extern void ld_setmatrix(uint8_t ledmatrix, uint8_t rows[8]);
extern void ld_setrow(uint8_t ledmatrix, uint8_t col, uint8_t row);
extern void ld_setled(uint8_t ledmatrix, uint8_t lednum, uint8_t status);
extern void ld_setledon(uint8_t ledmatrix, uint8_t lednum);
extern void ld_setledoff(uint8_t ledmatrix, uint8_t lednum);
extern void ld_setintensity(uint8_t ledmatrix, uint8_t intensity);
extern void ld_init();

#endif

