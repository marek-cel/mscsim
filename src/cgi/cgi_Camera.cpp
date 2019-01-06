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

#include <cgi/cgi_Camera.h>

#include <fdmUtils/fdm_WGS84.h>

#include <cgi/cgi_Intersections.h>
#include <cgi/cgi_WGS84.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Camera::Camera()
{
    m_manipulatorOrbit = new ManipulatorOrbit();
    m_manipulatorPilot = new ManipulatorPilot();
    m_manipulatorWorld = new ManipulatorWorld();

    m_viewType = Data::Camera::ViewPilot;
    m_manipulator = m_manipulatorPilot.get();
}

////////////////////////////////////////////////////////////////////////////////

Camera::~Camera() {}

////////////////////////////////////////////////////////////////////////////////

void Camera::update()
{
    if ( m_viewType == Data::Camera::ViewPilot )
    {
        osg::Quat q_tmp( -M_PI / 2.0, osg::X_AXIS,
                                 0.0, osg::Y_AXIS,
                          M_PI / 2.0, osg::Z_AXIS );

        osg::Quat q_wgs( Data::get()->ownship.att_ex_wgs,
                         Data::get()->ownship.att_ey_wgs,
                         Data::get()->ownship.att_ez_wgs,
                         Data::get()->ownship.att_e0_wgs );

        osg::Vec3d r_wgs( Data::get()->ownship.pos_x_wgs,
                          Data::get()->ownship.pos_y_wgs,
                          Data::get()->ownship.pos_z_wgs );

        osg::Matrixd matrix( osg::Matrixd::rotate( q_tmp )
                           * osg::Matrixd::rotate( q_wgs )
                           * osg::Matrixd::translate( r_wgs ) );

        m_manipulatorPilot->setByMatrix( matrix );
    }

    m_attitude = m_manipulator->getMatrix().getRotate();
    m_position = m_manipulator->getMatrix().getTrans();

    WGS84 wgs( m_position );

    float elevation = Intersections::instance()->getElevation( wgs.getLat(), wgs.getLon() );

    Data::get()->camera.latitude     = wgs.getLat();
    Data::get()->camera.longitude    = wgs.getLon();
    Data::get()->camera.altitude_agl = wgs.getAlt() - elevation;
    Data::get()->camera.altitude_asl = wgs.getAlt();

    Data::get()->camera.viewType = m_viewType;
}

////////////////////////////////////////////////////////////////////////////////

void Camera::setTrackNode( osg::Node *node )
{
    m_trackNode = node;

    m_manipulatorOrbit->setTrackNode( m_trackNode.get() );
}

////////////////////////////////////////////////////////////////////////////////

void Camera::setWorldNode( osg::Node *node )
{
    m_worldNode = node;

    m_manipulatorWorld->setNode( m_worldNode.get() );
    m_manipulatorWorld->setElevation( M_PI_2 );
    m_manipulatorWorld->setHeading( 0 );
}
