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

#include <fdm/utils/fdm_WGS84.h>

#include <cgi/cgi_Intersections.h>
#include <cgi/cgi_WGS84.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

const double Camera::m_downAngle = osg::DegreesToRadians( 10.0 );

////////////////////////////////////////////////////////////////////////////////

Camera::Camera()
{
    m_manipulatorOrbit = new ManipulatorOrbit();
    m_manipulatorShift = new ManipulatorShift();
    m_manipulatorWorld = new ManipulatorWorld();

    m_viewType = Data::Camera::ViewPilot;
    m_manipulator = m_manipulatorShift.get();
}

////////////////////////////////////////////////////////////////////////////////

Camera::~Camera() {}

////////////////////////////////////////////////////////////////////////////////

void Camera::update()
{
    if ( m_viewType == Data::Camera::ViewChase
      || m_viewType == Data::Camera::ViewPilot )
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

        if ( m_viewType == Data::Camera::ViewChase )
        {
            double d_phi = 0.0;
            double d_tht = 0.0;// -0.5 * Data::get()->ownship.pitchRate;
            double d_psi = 0.0;//  0.0 * Data::get()->ownship.yawRate;

            double d_x = -m_manipulatorShift->getDistance();
            double d_y = d_x * tan( d_psi ) * ( -1.0 );
            double d_z = d_x * tan( m_downAngle - d_tht );

            osg::Vec3d r_camera_bas( d_x, d_y, d_z );
            osg::Quat q_camera_bas( d_phi, osg::X_AXIS,
                                    d_tht, osg::Y_AXIS,
                                    d_psi, osg::Z_AXIS );

            osg::Vec3d r_camera_wgs = r_wgs + q_wgs * r_camera_bas;

            osg::Matrixd matrix( osg::Matrixd::rotate( q_tmp )
                               * osg::Matrixd::rotate( q_camera_bas )
                               * osg::Matrixd::rotate( q_wgs )
                               * osg::Matrixd::translate( r_camera_wgs ) );

            m_manipulatorShift->setByMatrix( matrix );
        }
        else if ( m_viewType == Data::Camera::ViewPilot )
        {
            osg::Matrixd matrix( osg::Matrixd::rotate( q_tmp )
                               * osg::Matrixd::rotate( q_wgs )
                               * osg::Matrixd::translate( r_wgs ) );

            m_manipulatorShift->setByMatrix( matrix );
        }
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

void Camera::setDistanceDef( double distance_def )
{
    m_manipulatorOrbit->setDistance( distance_def );
    m_manipulatorShift->setDistance( distance_def );
}

////////////////////////////////////////////////////////////////////////////////

void Camera::setDistanceMin( double distance_min )
{
    m_manipulatorOrbit->setDistanceMin( distance_min );
    m_manipulatorShift->setDistanceMin( distance_min );
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
