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

#include <fdm_c172/c172_GFC700_FD.h>

#include <fdm/fdm_Log.h>
#include <fdm/utils/fdm_Misc.h>
#include <fdm/utils/fdm_Units.h>
#include <fdm/utils/fdm_WGS84.h>
#include <fdm/xml/fdm_XmlUtils.h>

#include <fdm_c172/c172_GFC700_AP.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C172_GFC700_FD::C172_GFC700_FD( const C172_GFC700_AP *ap ) :
    FlightDirector( ap ),

    _ap ( ap ),

    _lat_mode ( LM_ROL ),
    _ver_mode ( VM_PIT )
{}

////////////////////////////////////////////////////////////////////////////////

C172_GFC700_FD::~C172_GFC700_FD() {}

////////////////////////////////////////////////////////////////////////////////

void C172_GFC700_FD::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_GFC700_FD::update( double timeStep,
                             double heading,
                             double altitude, double airspeed,
                             double turn_rate, double climb_rate,
                             double dme_distance,
                             double nav_deviation, bool nav_active,
                             double loc_deviation, bool loc_active,
                             double gs_deviation,  bool gs_active )
{

}

////////////////////////////////////////////////////////////////////////////////

void C172_GFC700_FD::toggleLatMode( LatMode lat_mode )
{
    if ( _lat_mode == lat_mode )
        _lat_mode = LM_ROL;
    else
        _lat_mode = lat_mode;
}

////////////////////////////////////////////////////////////////////////////////

void C172_GFC700_FD::toggleVerMode( VerMode ver_mode )
{
    if ( _ver_mode == ver_mode )
        _ver_mode = VM_PIT;
    else
        _ver_mode = ver_mode;
}
