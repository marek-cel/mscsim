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

#include <cgi/cgi_Manager.h>

#include <cgi/cgi_FindNode.h>
#include <cgi/cgi_Intersections.h>
#include <cgi/cgi_WGS84.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Manager::Manager() :
    _hud ( NULLPTR ),
    _map ( NULLPTR ),
    _otw ( NULLPTR ),

    _camera ( NULLPTR )
{
    _hud = new HUD();
    _map = new Map();
    _otw = new OTW();

    _camera = new Camera();
}

////////////////////////////////////////////////////////////////////////////////

Manager::~Manager()
{
    DELPTR( _hud );
    DELPTR( _map );
    DELPTR( _otw );

    DELPTR( _camera );
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateHUD()
{
    _hud->update();
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateMap()
{
    _map->update();
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateOTW()
{
    _otw->update();

    _camera->update();
    updateGround();
}

////////////////////////////////////////////////////////////////////////////////

void Manager::setCameraManipulatorChase()
{
    _camera->setViewChase();
}

////////////////////////////////////////////////////////////////////////////////

void Manager::setCameraManipulatorOrbit()
{
    if ( !_camera->isWorldNodeValid() )
    {
        osg::ref_ptr<osg::Node> ownshipNode = FindNode::findFirst( _otw->getNode(), "Ownship" );

        if ( ownshipNode.valid() )
        {
            _camera->setTrackNode( ownshipNode.get() );
        }
    }

    _camera->setViewOrbit();
}

////////////////////////////////////////////////////////////////////////////////

void Manager::setCameraManipulatorPilot()
{
    _camera->setViewPilot();
}

////////////////////////////////////////////////////////////////////////////////

void Manager::setCameraManipulatorWorld()
{
    if ( !_camera->isWorldNodeValid() )
    {
        osg::ref_ptr<osg::Node> sceneryNode = FindNode::findFirst( _otw->getNode(), "Scenery" );

        if ( sceneryNode.valid() )
        {
            _camera->setWorldNode( sceneryNode.get() );
        }
    }

    _camera->setViewWorld();
}

////////////////////////////////////////////////////////////////////////////////

void Manager::setDistanceDef( double distance_def )
{
    _camera->setDistanceDef( distance_def );
}

////////////////////////////////////////////////////////////////////////////////

void Manager::setDistanceMin( double distance_min )
{
    _camera->setDistanceMin( distance_min );
}

////////////////////////////////////////////////////////////////////////////////

void Manager::setMapScale( double scale )
{
    _map->setScale( scale );
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateGround()
{
    WGS84 wgs_b( Data::get()->ownship.latitude, Data::get()->ownship.longitude, 10000.0 );
    WGS84 wgs_e( Data::get()->ownship.latitude, Data::get()->ownship.longitude, -1000.0 );

    osg::Vec3d b = wgs_b.getPosition();
    osg::Vec3d e = wgs_e.getPosition();
    osg::Vec3d r;
    osg::Vec3d n;

    if ( Intersections::instance()->findFirst( b, e, r, n ) )
    {
        Data::get()->ground.elevation = WGS84( r ).getAlt();

        Data::get()->ground.r_x_wgs = r.x();
        Data::get()->ground.r_y_wgs = r.y();
        Data::get()->ground.r_z_wgs = r.z();
        Data::get()->ground.n_x_wgs = n.x();
        Data::get()->ground.n_y_wgs = n.y();
        Data::get()->ground.n_z_wgs = n.z();
    }
}
