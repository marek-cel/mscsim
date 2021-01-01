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

#include <g1000/sim/g1000_IFD.h>

#include <cstring>

////////////////////////////////////////////////////////////////////////////////

using namespace g1000;

////////////////////////////////////////////////////////////////////////////////

IFD::IFD() :
    _timeStep ( 0.0 ),

    _gia_1 ( NULLPTR ),
    _gia_2 ( NULLPTR ),

    _gdc ( NULLPTR ),
    _gea ( NULLPTR ),
    _grs ( NULLPTR ),
    _gmu ( NULLPTR ),
    _gtx ( NULLPTR )
{
    memset( &_input, 0, sizeof(Input) );

    _gia_1 = new GIA( this, 0 );
    _gia_2 = new GIA( this, 1 );

    _gdc = new GDC( this );
    _gea = new GEA( this );
    _grs = new GRS( this );
    _gmu = new GMU( this );
    _gtx = new GTX( this );
}

////////////////////////////////////////////////////////////////////////////////

IFD::~IFD()
{
    DELPTR( _gia_1 );
    DELPTR( _gia_2 );

    DELPTR( _gdc );
    DELPTR( _gea );
    DELPTR( _grs );
    DELPTR( _gmu );
    DELPTR( _gtx );
}

////////////////////////////////////////////////////////////////////////////////

void IFD::update( double timeStep, const Input &input )
{
    _timeStep = timeStep;

    memcpy( &_input, &input, sizeof(Input) );

    _gia_1->update();
    _gia_2->update();

    _gdc->update();
    _gea->update();
    _grs->update();
    _gmu->update();
    _gtx->update();
}
