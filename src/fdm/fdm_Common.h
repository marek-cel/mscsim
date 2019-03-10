/****************************************************************************//*
 * Copyright (C) 2019 Marek M. Cel
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
#ifndef FDM_COMMON_H
#define FDM_COMMON_H

////////////////////////////////////////////////////////////////////////////////

//#define FDM_TIME_STEP 0.01  /* 100 Hz */
#define FDM_TIME_STEP 0.005  /* 200 Hz */

#define FDM_TIME_STEP_MIN 0.001
#define FDM_TIME_STEP_MAX 0.1

////////////////////////////////////////////////////////////////////////////////

#define FDM_MAX_ENGINES 4

////////////////////////////////////////////////////////////////////////////////

#define FDM_MIN_INIT_ALTITUDE 30.0
#define FDM_MAX_INIT_STEPS 20000

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_COMMON_H
