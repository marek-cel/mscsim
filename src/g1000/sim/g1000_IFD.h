/****************************************************************************//*
 * Copyright (C) 2021 Marek M. Cel
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
#ifndef G1000_IFD_H
#define G1000_IFD_H

////////////////////////////////////////////////////////////////////////////////

#include <g1000/g1000_Defines.h>

#include <g1000/sim/g1000_GDC.h>
#include <g1000/sim/g1000_GEA.h>
#include <g1000/sim/g1000_GIA.h>
#include <g1000/sim/g1000_GRS.h>
#include <g1000/sim/g1000_GMU.h>
#include <g1000/sim/g1000_GTX.h>

////////////////////////////////////////////////////////////////////////////////

namespace g1000
{

/**
 * @brief G1000 Integrated Flight Deck simulation main class.
 */
class IFD
{
public:

    /** Constructor. */
    IFD();

    /** Destructor. */
    virtual ~IFD();

    /** Updates Integrated Flight Deck simulation. */
    void update( double timeStep, const Input &input );

    inline const Input& input() const { return _input; }

    inline double timeStep() const { return _timeStep; }

    inline const GIA* gia1() const { return _gia_1; }
    inline const GIA* gia2() const { return _gia_2; }

    inline const GDC* gdc() const { return _gdc; }
    inline const GEA* gea() const { return _gea; }
    inline const GRS* grs() const { return _grs; }
    inline const GMU* gmu() const { return _gmu; }
    inline const GTX* gtx() const { return _gtx; }

private:

    double _timeStep;   ///< [s] time step

    Input _input;       ///< input data

    GIA *_gia_1;        ///< GIA 63  Integrated Avionics Unit No. 1
    GIA *_gia_2;        ///< GIA 63  Integrated Avionics Unit No. 2

    GDC *_gdc;          ///< GDC 74A Air Data Computer
    GEA *_gea;          ///< GEA 71  Engine/Airframe Unit
    GRS *_grs;          ///< GRS 77  Attitude and Heading Reference System
    GMU *_gmu;          ///< GMU 44  Magnetometer
    GTX *_gtx;          ///< GTX 33  Transponder
};

} // end of g1000 namespace

////////////////////////////////////////////////////////////////////////////////

#endif // G1000_IFD_H
