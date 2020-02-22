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
#ifndef TEST_BLADE_H
#define TEST_BLADE_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/models/fdm_Blade.h>

#include <Data.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class test_Blade
{
public:

    typedef std::vector< Blade* > Blades;

    test_Blade( MainRotor::Direction direction, int blades_count );

    virtual ~test_Blade();

    void readData( const std::string &dataFile );

    void update( double timeStep );

    void updateData();

private:

    const int _bladesCount;

    const MainRotor::Direction _direction;

    Blade *_blade;

    Blades _blades;

    Matrix3x3 _ned2enu;
    Matrix3x3 _enu2ned;

    Matrix3x3 _bas2ned;
    Matrix3x3 _ned2bas;

    Matrix3x3 _bas2ras;
    Matrix3x3 _ras2bas;

    void updateDataVect( Data::Other::Vector &vect,
                         const Blade::Vect &out,
                         double azimuth );

};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // TEST_BLADE_H
