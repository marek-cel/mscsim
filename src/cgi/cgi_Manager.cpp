/****************************************************************************//*
 * Copyright (C) 2019 Marek M. Cel
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
    m_hud ( 0 ),
    m_map ( 0 ),
    m_otw ( 0 ),

    m_camera ( 0 )
{
    m_hud = new HUD();
    m_map = new Map();
    m_otw = new OTW();

    m_camera = new Camera();
}

////////////////////////////////////////////////////////////////////////////////

Manager::~Manager()
{
    if ( m_hud ) delete m_hud;
    m_hud = 0;

    if ( m_map ) delete m_map;
    m_map = 0;

    if ( m_otw ) delete m_otw;
    m_otw = 0;

    if ( m_camera ) delete m_camera;
    m_camera = 0;
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateHUD()
{
    m_hud->update();
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateMap()
{
    m_map->update();
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateOTW()
{
    m_otw->update();
    m_camera->update();
    updateGround();
}

////////////////////////////////////////////////////////////////////////////////

void Manager::setCameraManipulatorChase()
{
    m_camera->setViewChase();
}

////////////////////////////////////////////////////////////////////////////////

void Manager::setCameraManipulatorOrbit()
{
    if ( !m_camera->isWorldNodeValid() )
    {
        osg::ref_ptr<osg::Node> ownshipNode = FindNode::findFirst( m_otw->getNode(), "Ownship" );

        if ( ownshipNode.valid() )
        {
            m_camera->setTrackNode( ownshipNode.get() );
        }
    }

    m_camera->setViewOrbit();
}

////////////////////////////////////////////////////////////////////////////////

void Manager::setCameraManipulatorPilot()
{
    m_camera->setViewPilot();
}

////////////////////////////////////////////////////////////////////////////////

void Manager::setCameraManipulatorWorld()
{
    if ( !m_camera->isWorldNodeValid() )
    {
        osg::ref_ptr<osg::Node> sceneryNode = FindNode::findFirst( m_otw->getNode(), "Scenery" );

        if ( sceneryNode.valid() )
        {
            m_camera->setWorldNode( sceneryNode.get() );
        }
    }

    m_camera->setViewWorld();
}

////////////////////////////////////////////////////////////////////////////////

void Manager::setDistanceDef( double distance_def )
{
    m_camera->setDistanceDef( distance_def );
}

////////////////////////////////////////////////////////////////////////////////

void Manager::setDistanceMin( double distance_min )
{
    m_camera->setDistanceMin( distance_min );
}

////////////////////////////////////////////////////////////////////////////////

void Manager::setMapScale( double scale )
{
    m_map->setScale( scale );
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
