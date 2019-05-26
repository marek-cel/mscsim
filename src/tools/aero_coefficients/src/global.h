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
