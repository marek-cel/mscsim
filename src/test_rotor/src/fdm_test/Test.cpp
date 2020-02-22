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

#include <fdm_test/Test.h>

#include <fdm/fdm_Log.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Test::Test() :
    _blade ( nullptr ),
    _rotor ( nullptr )
{

}

////////////////////////////////////////////////////////////////////////////////

Test::~Test()
{
    stop();
}

////////////////////////////////////////////////////////////////////////////////

void Test::initBlades( MainRotor::Direction direction, int blades_count )
{
    if ( _blade == nullptr && _rotor == nullptr )
    {
        try
        {
            _blade = new test_Blade( direction, blades_count );
        }
        catch ( fdm::Exception &e )
        {
            fdm::Log::e() << e.getInfo() << std::endl;

            while ( e.hasCause() )
            {
                e = e.getCause();
                fdm::Log::e() << e.getInfo() << std::endl;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Test::update( double timeStep )
{
    if ( _blade )
    {
        _blade->update( timeStep );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Test::updateData()
{
    if ( _blade )
    {
        _blade->updateData();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Test::stop()
{
    if ( _blade ) delete _blade;
    _blade = nullptr;

    if ( _rotor ) delete _rotor;
    _rotor = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

bool Test::isInited()
{
    return ( _blade != nullptr && _rotor == nullptr )
        || ( _blade == nullptr && _rotor != nullptr );
}
