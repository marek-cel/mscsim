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

#include <cgi/cgi_Camera.h>

#include <cgi/cgi_Defines.h>

#include <fdm/utils/fdm_Misc.h>
#include <fdm/utils/fdm_WGS84.h>

#include <cgi/cgi_Intersections.h>
#include <cgi/cgi_WGS84.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

const double Camera::_downAngle = osg::DegreesToRadians( 10.0 );

////////////////////////////////////////////////////////////////////////////////

Camera::Camera() :
    _deltaPitch ( 0.0 )
{
    _manipulatorOrbit = new ManipulatorOrbit();
    _manipulatorShift = new ManipulatorShift();
    _manipulatorWorld = new ManipulatorWorld();

    _viewType = Data::CGI::ViewPilot;
    _manipulator = _manipulatorShift.get();
}

////////////////////////////////////////////////////////////////////////////////

Camera::~Camera() {}

////////////////////////////////////////////////////////////////////////////////

void Camera::update()
{
    if ( _viewType == Data::CGI::ViewChase || _viewType == Data::CGI::ViewPilot )
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

        if ( _viewType == Data::CGI::ViewChase )
        {
            double deltaPitch = Data::get()->ownship.onGround ? -Data::get()->ownship.pitch : 0.0;
            _deltaPitch = fdm::Misc::inertia( deltaPitch, _deltaPitch, CGI_TIME_STEP, 1.0 );

            double d_phi = 0.0;
            double d_tht = _deltaPitch;// -0.5 * Data::get()->ownship.pitchRate;
            double d_psi = 0.0;//  0.0 * Data::get()->ownship.yawRate;

            double d_x = -_manipulatorShift->getDistance();
            double d_y = d_x * tan( d_psi ) * ( -1.0 );
            double d_z = d_x * tan( _downAngle - d_tht );

            osg::Vec3d r_camera_bas( d_x, d_y, d_z );
            osg::Quat q_camera_bas( d_phi, osg::X_AXIS,
                                    d_tht, osg::Y_AXIS,
                                    d_psi, osg::Z_AXIS );

            osg::Vec3d r_camera_wgs = r_wgs + q_wgs * r_camera_bas;

            osg::Matrixd matrix( osg::Matrixd::rotate( q_tmp )
                               * osg::Matrixd::rotate( q_camera_bas )
                               * osg::Matrixd::rotate( q_wgs )
                               * osg::Matrixd::translate( r_camera_wgs ) );

            _manipulatorShift->setByMatrix( matrix );
        }
        else if ( _viewType == Data::CGI::ViewPilot )
        {
            osg::Vec3d r_camera_bas( Data::get()->cgi.camera.offset_x,
                                     Data::get()->cgi.camera.offset_y,
                                     Data::get()->cgi.camera.offset_z );

            osg::Vec3d r_camera_wgs = r_wgs + q_wgs * r_camera_bas;

            osg::Matrixd matrix( osg::Matrixd::rotate( q_tmp )
                               * osg::Matrixd::rotate( q_wgs )
                               * osg::Matrixd::translate( r_camera_wgs ) );

            _manipulatorShift->setByMatrix( matrix );
        }
    }

    //if ( _viewType != Data::Camera::ViewChase ) _deltaPitch = 0.0;

    _attitude = _manipulator->getMatrix().getRotate();
    _position = _manipulator->getMatrix().getTrans();

    WGS84 wgs( _position );

    double elevation = Intersections::instance()->getElevation( wgs.getLat(), wgs.getLon() );

    Data::get()->cgi.camera.latitude     = wgs.getLat();
    Data::get()->cgi.camera.longitude    = wgs.getLon();
    Data::get()->cgi.camera.altitude_agl = wgs.getAlt() - elevation;
    Data::get()->cgi.camera.altitude_asl = wgs.getAlt();

    Data::get()->cgi.viewType = _viewType;
}

////////////////////////////////////////////////////////////////////////////////

void Camera::setDistanceDef( double distance_def )
{
    _manipulatorOrbit->setDistance( distance_def );
    _manipulatorShift->setDistance( distance_def );
}

////////////////////////////////////////////////////////////////////////////////

void Camera::setDistanceMin( double distance_min )
{
    _manipulatorOrbit->setDistanceMin( distance_min );
    _manipulatorShift->setDistanceMin( distance_min );
}

////////////////////////////////////////////////////////////////////////////////

void Camera::setTrackNode( osg::Node *node )
{
    _trackNode = node;

    _manipulatorOrbit->setTrackNode( _trackNode.get() );
}

////////////////////////////////////////////////////////////////////////////////

void Camera::setWorldNode( osg::Node *node )
{
    _worldNode = node;

    _manipulatorWorld->setNode( _worldNode.get() );
    _manipulatorWorld->setElevation( M_PI_2 );
    _manipulatorWorld->setHeading( 0 );
}
