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

#include <fdm_test/test_Rotor.h>

#include <fdm/fdm_Log.h>
#include <fdm/xml/fdm_XmlDoc.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

test_Rotor::test_Rotor( MainRotor::Direction direction, int blades_count ) :
    _bladesCount ( blades_count ),
    _direction ( direction ),
    _rotor ( FDM_NULLPTR )
{
    _rotor = new MainRotorBE();

    readData( "../data/test.xml" );

    _ned2enu(0,0) =  0.0;
    _ned2enu(0,1) =  1.0;
    _ned2enu(0,2) =  0.0;

    _ned2enu(1,0) =  1.0;
    _ned2enu(1,1) =  0.0;
    _ned2enu(1,2) =  0.0;

    _ned2enu(2,0) =  0.0;
    _ned2enu(2,1) =  0.0;
    _ned2enu(2,2) = -1.0;

    _enu2ned = _ned2enu.getTransposed();

    _bas2ned = Matrix3x3::createIdentityMatrix();
    _ned2bas = Matrix3x3::createIdentityMatrix();
}

////////////////////////////////////////////////////////////////////////////////

test_Rotor::~test_Rotor()
{
    FDM_DELPTR( _rotor );
}

////////////////////////////////////////////////////////////////////////////////

void test_Rotor::readData( const std::string &dataFile )
{
    XmlDoc doc( dataFile );

    if ( doc.isOpen() )
    {
        XmlNode rootNode = doc.getRootNode();

        if ( rootNode.isValid() )
        {
            XmlNode nodeRotor = rootNode.getFirstChildElement( "main_rotor" );

            _rotor->readData( nodeRotor );
        }
        else
        {
            Log::e() << "Reading file \"" << dataFile << "\" failed. Invalid root node." << std::endl;
        }
    }
    else
    {
        Log::e() << "Reading file \"" << dataFile << "\" failed." << std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////

void test_Rotor::update( double timeStep )
{

}

////////////////////////////////////////////////////////////////////////////////

void test_Rotor::updateData()
{

}
