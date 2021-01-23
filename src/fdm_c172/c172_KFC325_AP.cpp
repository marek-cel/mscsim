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

#include <fdm_c172/c172_KFC325_AP.h>

#include <fdm/fdm_Path.h>

#include <fdm/xml/fdm_XmlDoc.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C172_KFC325_AP::C172_KFC325_AP() :
    Autopilot ( _fd = new C172_KFC325_FD( this ) ),

    _rate_pitch ( 0.0 ),
    _rate_alt   ( 0.0 ),
    _rate_ias   ( 0.0 ),
    _rate_vs    ( 0.0 ),

    _softRideCoef ( 0.0 ),

    _softRide ( false )
{}

////////////////////////////////////////////////////////////////////////////////

C172_KFC325_AP::~C172_KFC325_AP()
{
    FDM_DELPTR( _fd );
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_AP::readData( XmlNode &dataNode )
{
    ////////////////////////////////
    Autopilot::readData( dataNode );
    ////////////////////////////////

    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_rate_pitch , "rate_pitch" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_rate_alt   , "rate_alt"   );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_rate_ias   , "rate_ias"   );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_rate_vs    , "rate_vs"    );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_softRideCoef, "soft_ride_coef" );

        if ( result != FDM_SUCCESS )
        {
            XmlUtils::throwError( __FILE__, __LINE__, dataNode );
        }
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_AP::initialize()
{
    std::string dataFile = Path::get( "fdm/c172/c172_ap_kfc325.xml" );
    fdm::XmlDoc doc( dataFile.c_str() );

    if ( doc.isOpen() )
    {
        fdm::XmlNode rootNode = doc.getRootNode();

        if ( rootNode.isValid() )
        {
            readData( rootNode );
        }
    }
    else
    {
        Exception e;

        e.setType( Exception::FileReadingError );
        e.setInfo( "Reading file \"" + dataFile + "\" failed." );

        FDM_THROW( e );
    }

    ////////////////////////
    Autopilot::initialize();
    ////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_AP::update( double timeStep, bool button_dn, bool button_up )
{
    if ( button_dn || button_up )
    {
        double coef = button_up ? 1.0 : -1.0;

        switch ( _fd->getVerMode() )
        {
        case C172_KFC325_FD::VM_FD:
            setPitch( getPitch() + coef * timeStep * _rate_pitch );
            break;

        case C172_KFC325_FD::VM_ALT:
            setAltitude( getAltitude() + coef * timeStep * _rate_alt );
            break;

        case C172_KFC325_FD::VM_IAS:
            setAirspeed( getAirspeed() + coef * timeStep * _rate_ias );
            break;

        case C172_KFC325_FD::VM_VS:
            setClimbRate( getClimbRate() + coef * timeStep * _rate_vs );
            break;

        case C172_KFC325_FD::VM_ARM:
        default:
            // do nothing
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_AP::onPressedAP()
{
    _engaged = !_engaged;

    if ( _engaged ) _yaw_damper = true;
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_AP::onPressedFD()
{
    _fd->onPressedFD();
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_AP::onPressedALT()
{
    _fd->onPressedALT();
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_AP::onPressedIAS()
{
    _fd->onPressedIAS();
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_AP::onPressedENG()
{
    _fd->onPressedENG();
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_AP::onPressedARM()
{
    _fd->onPressedARM();
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_AP::onPressedHDG()
{
    _fd->onPressedHDG();
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_AP::onPressedNAV()
{
    _fd->onPressedNAV();
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_AP::onPressedAPR()
{
    _fd->onPressedAPR();
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_AP::onPressedBC()
{
    _fd->onPressedBC();
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_AP::onPressedYD()
{
    _yaw_damper = !_yaw_damper;
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_AP::onPressedSoftRide()
{
    _softRide = !_softRide;
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_AP::onPressedHalfBank()
{
    _fd->onPressedHalfBank();
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_AP::onPressedTest()
{
    _testing = true;
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_AP::onReleasedTest()
{
    _testing = false;
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_AP::setHeadingILS( double heading_ils )
{
    _fd->setHeadingILS( heading_ils );
}
