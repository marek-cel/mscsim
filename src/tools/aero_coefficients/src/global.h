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
#ifndef GLOBAL_H
#define GLOBAL_H

////////////////////////////////////////////////////////////////////////////////

#include <string>

#include "CoefDrag.h"
#include "CoefLift.h"

////////////////////////////////////////////////////////////////////////////////

#ifdef MAIN_CPP

std::string liftInp;
std::string liftOut;
std::string dragInp;
std::string dragOut;

CoefDrag *drag = 0;
CoefLift *lift = 0;

double al_s = 0.0;
double al_1 = 0.0;
double al_2 = 0.0;

double cl_s = 0.0;
double cl_1 = 0.0;
double cl_2 = 0.0;

double ad_1 = 0.0;
double ad_2 = 0.0;
double ad_3 = 0.0;
double ad_4 = 0.0;

double cd_0 = 0.0;
double cd_1 = 0.0;
double cd_2 = 0.0;
double cd_3 = 0.0;
double cd_4 = 0.0;
double cd_5 = 0.0;

#else

extern std::string liftInp;
extern std::string liftOut;
extern std::string dragInp;
extern std::string dragOut;

extern CoefDrag *drag;
extern CoefLift *lift;

extern double al_s;
extern double al_1;
extern double al_2;

extern double cl_s;
extern double cl_1;
extern double cl_2;

extern double ad_1;
extern double ad_2;
extern double ad_3;
extern double ad_4;

extern double cd_0;
extern double cd_1;
extern double cd_2;
extern double cd_3;
extern double cd_4;
extern double cd_5;

#endif // MAIN_CPP

////////////////////////////////////////////////////////////////////////////////

#endif // GLOBAL_H
