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

#include <cgi/cgi_Fuselage.h>

#include <cgi/cgi_Models.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Fuselage::Fuselage() :
    _direction ( Data::Rotor::CW )
{
    _root = new osg::Group();

    reload();
}

////////////////////////////////////////////////////////////////////////////////

Fuselage::~Fuselage() {}

////////////////////////////////////////////////////////////////////////////////

void Fuselage::update()
{
    if ( _direction != Data::get()->rotor.direction )
    {
        reload();
    }

    _direction = Data::get()->rotor.direction;
}

////////////////////////////////////////////////////////////////////////////////

void Fuselage::reload()
{
    removeAllChildren();

    osg::ref_ptr<osg::Node> nodeHeli;

    if ( Data::get()->rotor.direction == Data::Rotor::CCW )
        nodeHeli = Models::get( "../data/uh60_fuselage.osgt" );
    else
        nodeHeli = Models::get( "../data/w3wa_fuselage.osgt" );

    if ( nodeHeli.valid() ) _root->addChild( nodeHeli.get() );
}

////////////////////////////////////////////////////////////////////////////////

void Fuselage::removeAllChildren()
{
    if ( _root.valid() )
    {
        _root->removeChild( 0, _root->getNumChildren() );
    }
}

