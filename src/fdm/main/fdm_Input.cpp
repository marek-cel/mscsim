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

#include <fdm/main/fdm_Input.h>

#include <fdm/fdm_Exception.h>

#include <fdm/utils/fdm_String.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Input::Input() {}

////////////////////////////////////////////////////////////////////////////////

Input::~Input() {}

////////////////////////////////////////////////////////////////////////////////

void Input::init()
{
    int result = FDM_SUCCESS;

    // input - controls
    if ( result == FDM_SUCCESS ) result = addNode( "controls.roll"         , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addNode( "controls.pitch"        , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addNode( "controls.yaw"          , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addNode( "controls.trim_roll"    , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addNode( "controls.trim_pitch"   , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addNode( "controls.trim_yaw"     , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addNode( "controls.brake_left"   , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addNode( "controls.brake_right"  , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addNode( "controls.wheel_brake"  , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addNode( "controls.landing_gear" , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addNode( "controls.wheel_nose"   , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addNode( "controls.flaps"        , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addNode( "controls.airbrake"     , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addNode( "controls.spoilers"     , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addNode( "controls.collective"   , DataNode::Double );

    if ( result == FDM_SUCCESS ) result = addNode( "controls.lgh" , DataNode::Bool );
    if ( result == FDM_SUCCESS ) result = addNode( "controls.nws" , DataNode::Bool );
    if ( result == FDM_SUCCESS ) result = addNode( "controls.abs" , DataNode::Bool );

    // input - engines
    for ( int i = 0; i < FDM_MAX_ENGINES; i++ )
    {
        std::string number = String::toString( i + 1 );

        std::string idstr_throttle  = "engine_" + number + ".throttle";
        std::string idstr_mixture   = "engine_" + number + ".mixture";
        std::string idstr_propeller = "engine_" + number + ".propeller";
        std::string idstr_fuel      = "engine_" + number + ".fuel";
        std::string idstr_ignition  = "engine_" + number + ".ignition";
        std::string idstr_starter   = "engine_" + number + ".starter";

        if ( result == FDM_SUCCESS ) result = addNode( idstr_throttle  .c_str() , DataNode::Double );
        if ( result == FDM_SUCCESS ) result = addNode( idstr_mixture   .c_str() , DataNode::Double );
        if ( result == FDM_SUCCESS ) result = addNode( idstr_propeller .c_str() , DataNode::Double );

        if ( result == FDM_SUCCESS ) result = addNode( idstr_fuel     .c_str()  , DataNode::Bool );
        if ( result == FDM_SUCCESS ) result = addNode( idstr_ignition .c_str()  , DataNode::Bool );
        if ( result == FDM_SUCCESS ) result = addNode( idstr_starter  .c_str()  , DataNode::Bool );
    }

    // input - masses
    for ( int i = 0; i < FDM_MAX_PILOTS; i++ )
    {
        std::string idstr = "masses.pilot_" + String::toString( i + 1 );

        if ( result == FDM_SUCCESS ) result = addNode( idstr.c_str(), DataNode::Double );
    }

    for ( int i = 0; i < FDM_MAX_TANKS; i++ )
    {
        std::string idstr = "masses.tank_" + String::toString( i + 1 );

        if ( result == FDM_SUCCESS ) result = addNode( idstr.c_str(), DataNode::Double );
    }

    if ( result == FDM_SUCCESS ) result = addNode( "masses.cabin" , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addNode( "masses.trunk" , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addNode( "masses.slung" , DataNode::Double );

    if ( result != FDM_SUCCESS )
    {
        Exception e;

        e.setType( Exception::DataRefInitError );
        e.setInfo( "Cannot initialize data tree." );

        FDM_THROW( e );
    }
}
