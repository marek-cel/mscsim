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
#ifndef TEST_H
#define TEST_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm_test/test_Blade.h>
#include <fdm_test/test_Rotor.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class Test
{
public:

    Test();

    virtual ~Test();

    void initBlade( MainRotor::Direction direction, int blades_count );
    void initRotor();

    void update( double timeStep );

    void updateData();

    void stop();

    bool isInited();

private:

    test_Blade *_blade;
    test_Rotor *_rotor;

};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // TEST_H
