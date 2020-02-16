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

#include <cgi/cgi_Windsock.h>

#include <cgi/cgi_Models.h>

#include <Data.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Windsock::Windsock()
{
    _root = new osg::Group();

    createWindsock();
}

////////////////////////////////////////////////////////////////////////////////

Windsock::~Windsock() {}

////////////////////////////////////////////////////////////////////////////////

void Windsock::update()
{
    if ( Data::get()->state.wind_vel > 1.0e-9 )
    {
        _pat->setAttitude( osg::Quat( -Data::get()->state.wind_dir, osg::Z_AXIS ) );
    }
    else
    {
        _pat->setAttitude( osg::Quat( M_PI_2, osg::X_AXIS ) );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Windsock::createWindsock()
{
    const double x =  20.0;
    const double y =  20.0;
    const double z = -10.0;

    osg::ref_ptr<osg::PositionAttitudeTransform> pat1 = new osg::PositionAttitudeTransform();
    osg::ref_ptr<osg::PositionAttitudeTransform> pat2 = new osg::PositionAttitudeTransform();
    osg::ref_ptr<osg::PositionAttitudeTransform> pat3 = new osg::PositionAttitudeTransform();
    osg::ref_ptr<osg::PositionAttitudeTransform> pat4 = new osg::PositionAttitudeTransform();

    _root->addChild( pat1.get() );
    _root->addChild( pat2.get() );
    _root->addChild( pat3.get() );
    _root->addChild( pat4.get() );

    pat1->setPosition( osg::Vec3( -x, -y, z ) );
    pat2->setPosition( osg::Vec3(  x, -y, z ) );
    pat3->setPosition( osg::Vec3(  x,  y, z ) );
    pat4->setPosition( osg::Vec3( -x,  y, z ) );

    osg::ref_ptr<osg::Node> nodePole = Models::get( "../data/wind_pole.osgt" );

    if ( nodePole.valid() )
    {
        pat1->addChild( nodePole.get() );
        pat2->addChild( nodePole.get() );
        pat3->addChild( nodePole.get() );
        pat4->addChild( nodePole.get() );
    }

    osg::ref_ptr<osg::PositionAttitudeTransform> pat0 = new osg::PositionAttitudeTransform();
    pat0->setPosition( osg::Vec3( 0.0, 0.0, 3.0 ) );

    pat1->addChild( pat0.get() );
    pat2->addChild( pat0.get() );
    pat3->addChild( pat0.get() );
    pat4->addChild( pat0.get() );

    _pat = new osg::PositionAttitudeTransform();
    pat0->addChild( _pat.get() );

    osg::ref_ptr<osg::Node> nodeSock = Models::get( "../data/wind_sock.osgt" );

    if ( nodeSock.valid() )
    {
        _pat->addChild( nodeSock.get() );
    }
}
