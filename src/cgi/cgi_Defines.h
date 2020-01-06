/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ******************************************************************************/
#ifndef CGI_DEFINES_H
#define CGI_DEFINES_H

////////////////////////////////////////////////////////////////////////////////

#include <Defines.h>

////////////////////////////////////////////////////////////////////////////////

#define CGI_TIME_STEP 0.017 /*  60 Hz */

////////////////////////////////////////////////////////////////////////////////

#define CGI_FOV_Y 30.0

#define CGI_HUD_Y 200.0
#define CGI_HUD_Y_2 ( CGI_HUD_Y / 2.0 )

#define CGI_MAP_FOV_Y 15.0
#define CGI_MAP_Y_2 1.0e7

////////////////////////////////////////////////////////////////////////////////

#define CGI_DEPTH_SORTED_BIN_WORLD  0
#define CGI_DEPTH_SORTED_BIN_SKY    1
#define CGI_DEPTH_SORTED_BIN_MOON   2
#define CGI_DEPTH_SORTED_BIN_SUN    2
#define CGI_DEPTH_SORTED_BIN_STARS  2
#define CGI_DEPTH_SORTED_BIN_CLOUDS 3
#define CGI_DEPTH_SORTED_BIN_HUD    4

////////////////////////////////////////////////////////////////////////////////

#define CGI_DEPTH_SORTED_BIN_MAP   0
#define CGI_DEPTH_SORTED_BIN_ICONS 1

////////////////////////////////////////////////////////////////////////////////

#define CGI_CLOUDS_MAX_COUNT 2048
#define CGI_CLOUDS_MAX_SPRITES 64

////////////////////////////////////////////////////////////////////////////////

#define CGI_FOG_LIMIT 10000.0f

////////////////////////////////////////////////////////////////////////////////

#define CGI_SKYDOME_RADIUS 25000.0f

#define CGI_SKYDOME_SCALING_TRANSIENT_ALT_MIN  10.0f
#define CGI_SKYDOME_SCALING_TRANSIENT_ALT_MAX 100.0f

#define CGI_SKYDOME_DIAMETER_SUN  0.54f
#define CGI_SKYDOME_DIAMETER_MOON 0.53f

////////////////////////////////////////////////////////////////////////////////

#define CGI_LIGHT_SUN_NUM  0
#define CGI_LIGHT_MOON_NUM 1

////////////////////////////////////////////////////////////////////////////////

#define CGI_MAX_LANDING_GEAR_ELEMENTS 20
#define CGI_MAX_FLAPS_ELEMENTS 10

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_DEFINES_H
