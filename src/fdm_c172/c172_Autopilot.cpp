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

#include <fdm_c172/c172_Autopilot.h>

#include <fdm/fdm_Path.h>

#include <fdm/xml/fdm_XmlDoc.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C172_Autopilot::C172_Autopilot() :
    Autopilot ( _fd = new C172_FlightDirector() ),

    _rate_pitch ( 0.0 ),
    _rate_alt   ( 0.0 ),
    _rate_ias   ( 0.0 ),
    _rate_vs    ( 0.0 ),

    _softRideCoef ( 0.0 ),

    _softRide ( false ),
    _halfBank ( false )
{}

////////////////////////////////////////////////////////////////////////////////

C172_Autopilot::~C172_Autopilot()
{
    FDM_DELPTR( _fd );
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::init()
{
    std::string dataFile = Path::get( "data/fdm/c172/c172_ap.xml" );
    fdm::XmlDoc doc( dataFile );

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

    //////////////////
    Autopilot::init();
    //////////////////
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::readData( XmlNode &dataNode )
{
    ////////////////////////////////
    Autopilot::readData( dataNode );
    ////////////////////////////////

    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _rate_pitch , "rate_pitch" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _rate_alt   , "rate_alt"   );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _rate_ias   , "rate_ias"   );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _rate_vs    , "rate_vs"    );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _softRideCoef, "soft_ride_coef" );

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

void C172_Autopilot::update( double timeStep, bool button_dn, bool button_up )
{
    if ( button_dn || button_up )
    {
        double coef = button_up ? 1.0 : -1.0;

        switch ( _fd->getVerMode() )
        {
        case C172_FlightDirector::VM_FD:
            setPitch( getPitch() + coef * timeStep * _rate_pitch );
            break;

        case C172_FlightDirector::VM_ALT:
            setAltitude( getAltitude() + coef * timeStep * _rate_alt );
            break;

        case C172_FlightDirector::VM_IAS:
            setAirspeed( getAirspeed() + coef * timeStep * _rate_ias );
            break;

        case C172_FlightDirector::VM_VS:
            setClimbRate( getClimbRate() + coef * timeStep * _rate_vs );
            break;

        case C172_FlightDirector::VM_ARM:
        default:
            // do nothing
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedAP()
{
    _engaged = !_engaged;

    if ( _engaged ) _yawDamper = true;
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedFD()
{
    if ( _fd->isEngaged() )
    {
        _fd->disengage();

        if ( !_engaged )
        {
            _fd->setLatMode( C172_FlightDirector::LM_FD );
            _fd->setVerMode( C172_FlightDirector::VM_FD );
        }

    }
    else
    {
        _fd->engage();
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedALT()
{
    if ( !isActiveGS() )
        _fd->toggleVerMode( C172_FlightDirector::VM_ALT );
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedIAS()
{
    if ( !isActiveGS() )
        _fd->toggleVerMode( C172_FlightDirector::VM_IAS );
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedENG()
{
    if ( !isActiveGS() )
        _fd->setVerMode( C172_FlightDirector::VM_VS ); // or toggle ??
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedARM()
{
    if ( !isActiveGS() )
        _fd->setVerMode( C172_FlightDirector::VM_ARM ); // or toggle ??
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedHDG()
{
    _fd->engage();
    _fd->toggleLatMode( C172_FlightDirector::LM_HDG );
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedNAV()
{
    _fd->engage();

    if ( isActiveNAV() && !isArmedNAV() )
        _fd->toggleLatMode( C172_FlightDirector::LM_NAV );
    else
        _fd->setArmMode( C172_FlightDirector::ARM_NAV );
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedAPR()
{
    _fd->engage();

    if ( isActiveAPR() && !isArmedAPR() )
    {
        _fd->setLatMode( C172_FlightDirector::LM_FD );

        if ( isActiveGS() )
            _fd->setVerMode( C172_FlightDirector::VM_FD );
    }
    else
        _fd->setArmMode( C172_FlightDirector::ARM_APR );
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedBC()
{
    _fd->engage();

    if ( isActiveBC() && !isArmedBC() )
    {
        _fd->setLatMode( C172_FlightDirector::LM_FD );

        if ( isActiveGS() )
            _fd->setVerMode( C172_FlightDirector::VM_FD );
    }
    else
        _fd->setArmMode( C172_FlightDirector::ARM_BC );
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedYD()
{
    _yawDamper = !_yawDamper;
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedSoftRide()
{
    _softRide = !_softRide;
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedHalfBank()
{
    _halfBank = !_halfBank;

    if ( _halfBank )
        _fd->enableHalfBank();
    else
        _fd->disableHalfBank();
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedTest()
{
    _testing = true;
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onReleasedTest()
{
    _testing = false;
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::setHeadingILS( double heading_ils )
{
    _fd->setHeadingILS( heading_ils );
}
