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

#include <fdm_f16c/f16c_Aircraft.h>

#include <fdmXml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

F16C_Propulsion::F16C_Propulsion( const F16C_Aircraft *aircraft ) :
    Propulsion( aircraft ),
    m_aircraft ( aircraft )
{}

////////////////////////////////////////////////////////////////////////////////

F16C_Propulsion::~F16C_Propulsion() {}

////////////////////////////////////////////////////////////////////////////////

void F16C_Propulsion::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        // TODO
    }
    else
    {
        Exception e;

        e.setType( Exception::FileReadingError );
        e.setInfo( "ERROR! Reading XML file failed. " + XmlUtils::getErrorInfo( dataNode ) );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void F16C_Propulsion::initialize( bool engineOn )
{
    // TODO
}

////////////////////////////////////////////////////////////////////////////////

void F16C_Propulsion::initDataRefs()
{
    ///////////////////////////
    Propulsion::initDataRefs();
    ///////////////////////////

    int result = FDM_SUCCESS;

    // inputs
    if ( result == FDM_SUCCESS ) result = addDataRef( "input/propulsion/throttle" , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input/propulsion/fuel"     , DataNode::Bool   );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input/propulsion/ignition" , DataNode::Bool   );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input/propulsion/starter"  , DataNode::Bool   );

    if ( result == FDM_SUCCESS )
    {
        m_drThrottle  = getDataRef( "input/propulsion/throttle" );
        m_drFuel      = getDataRef( "input/propulsion/fuel"     );
        m_drIgnition  = getDataRef( "input/propulsion/ignition" );
        m_drStarter   = getDataRef( "input/propulsion/starter"  );
    }
    else
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "ERROR! Initializing data references failed." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void F16C_Propulsion::computeForceAndMoment()
{
    m_for_bas.x() = 131223.0 * m_drThrottle.getValue();

    if ( !m_for_bas.isValid() || !m_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "ERROR! NaN detected in the propulsion model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void F16C_Propulsion::update()
{
    // TODO
}
