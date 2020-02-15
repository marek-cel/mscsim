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

#include <cgi/cgi_Helicopter.h>

#include <Data.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Helicopter::Helicopter() :
    _fuselage ( nullptr ),
    _rotor ( nullptr )
{
    _root = new osg::Group();

    osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
    _root->addChild( pat.get() );
    pat->setAttitude( osg::Quat( M_PI, osg::X_AXIS,
                                  0.0, osg::Y_AXIS,
                               M_PI_2, osg::Z_AXIS ) );

    _pat = new osg::PositionAttitudeTransform();
    pat->addChild( _pat.get() );

    _fuselage = new Fuselage();
    _rotor = new Rotor();

    _pat->addChild( _fuselage->getRoot() );
    _pat->addChild( _rotor->getRoot() );
}

////////////////////////////////////////////////////////////////////////////////

Helicopter::~Helicopter()
{
    if ( _fuselage ) delete _fuselage;
    _fuselage = nullptr;

    if ( _rotor ) delete _rotor;
    _rotor = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

void Helicopter::update()
{
    _fuselage->update();
    _rotor->update();

    _pat->setAttitude( osg::Quat( Data::get()->state.ned_phi, osg::X_AXIS,
                                  Data::get()->state.ned_tht, osg::Y_AXIS,
                                  Data::get()->state.ned_psi, osg::Z_AXIS ) );
}
