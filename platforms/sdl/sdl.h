/****************************************************************************
 *     _____        _____ _   _ _______   _____  _    _ _   _
 *    |  __ \ /\   |_   _| \ | |__   __| |  __ \| |  | | \ | |
 *    | |__) /  \    | | |  \| |  | |    | |__) | |  | |  \| |
 *    |  ___/ /\ \   | | | . ` |  | |    |  _  /| |  | | . ` |
 *    | |  / ____ \ _| |_| |\  |  | |    | | \ \| |__| | |\  |
 *    |_| /_/    \_\_____|_| \_|  |_|    |_|  \_\\____/|_| \_|
 *
 * Copyright (C) 2015 Franklin Wei
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

/* here's what a platform header needs to have #defined: */
/* FRACBITS - number of fractional bits to be used */
/* FRACBITS must be at least 7 or else the game will function improperly */

/* LCD_WIDTH - screen width */
/* LCD_HEIGHT - screen height */
/* LCD_RGBPACK(r,g,b) - packs RGB value into an integer */
/* FP_MUL(x,y) - fixed-point multiply with FRACBITS fractional bits */
/* FP_DIV(x,y) - fixed-point divide */

#ifndef _SDL_PLAT_H_
#define _SDL_PLAT_H_

#include <SDL/SDL.h>

#include <assert.h>

#define FRACBITS 20
#define LCD_WIDTH 640
#define LCD_HEIGHT 480

unsigned sdlplat_rgbpack(uint8_t, uint8_t, uint8_t);

#define LCD_RGBPACK(r,g,b) sdlplat_rgbpack(r,g,b)

#define FP_MUL(x,y) (long)((((long long)(x)) * ((long long)(y))) >> FRACBITS)
#define FP_DIV(x,y) (long)((((long long)(x)) << FRACBITS) / ((long long)(y)))

/* disable this to enhance performance */
#define PLAT_WANTS_YIELD

typedef unsigned color_t;

#endif
