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

#include <cgi/otw/cgi_Airport.h>

#include <Data.h>

#include <cgi/cgi_FindNode.h>
#include <cgi/cgi_Models.h>
#include <cgi/cgi_WGS84.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Airport::Airport( const std::string &file, double lat, double lon, double alt,
                  Module *parent ) :
    Module( parent )
{
    _pat = new osg::PositionAttitudeTransform();
    _root->addChild( _pat.get() );

    osg::ref_ptr<osg::Node> airportNode = Models::get( file );

    if ( airportNode.valid() )
    {
        _pat->addChild( airportNode.get() );

        // position
        WGS84 wgs( lat, lon, alt );

        _pat->setPosition( wgs.getPosition() );
        _pat->setAttitude( wgs.getAttitude() );

        _switchLightsHELI = dynamic_cast<osg::Switch*>( FindNode::findFirst( airportNode, "HELI" ) );
        _switchLightsRALS = dynamic_cast<osg::Switch*>( FindNode::findFirst( airportNode, "RALS" ) );
        _switchLightsRCLS = dynamic_cast<osg::Switch*>( FindNode::findFirst( airportNode, "RCLS" ) );
        _switchLightsRELS = dynamic_cast<osg::Switch*>( FindNode::findFirst( airportNode, "RELS" ) );
        _switchLightsTDZL = dynamic_cast<osg::Switch*>( FindNode::findFirst( airportNode, "TDZL" ) );
        _switchLightsTELS = dynamic_cast<osg::Switch*>( FindNode::findFirst( airportNode, "TELS" ) );
        _switchLightsTWRL = dynamic_cast<osg::Switch*>( FindNode::findFirst( airportNode, "TWRL" ) );
        _switchLightsVGSI = dynamic_cast<osg::Switch*>( FindNode::findFirst( airportNode, "VGSI" ) );
        _switchGatesRwy18 = dynamic_cast<osg::Switch*>( FindNode::findFirst( airportNode, "ApproachGates18" ) );
        _switchGatesRwy36 = dynamic_cast<osg::Switch*>( FindNode::findFirst( airportNode, "ApproachGates36" ) );
    }
}

////////////////////////////////////////////////////////////////////////////////

Airport::~Airport() {}

////////////////////////////////////////////////////////////////////////////////

void Airport::update()
{
    /////////////////
    Module::update();
    /////////////////

    if ( _switchLightsHELI.valid() )
    {
        if ( Data::get()->cgi.airport.lightsHELI )
            _switchLightsHELI->setAllChildrenOn();
        else
            _switchLightsHELI->setAllChildrenOff();
    }

    if ( _switchLightsRALS.valid() )
    {
        if ( Data::get()->cgi.airport.lightsRALS )
            _switchLightsRALS->setAllChildrenOn();
        else
            _switchLightsRALS->setAllChildrenOff();
    }

    if ( _switchLightsRCLS.valid() )
    {
        if ( Data::get()->cgi.airport.lightsRCLS )
            _switchLightsRCLS->setAllChildrenOn();
        else
            _switchLightsRCLS->setAllChildrenOff();
    }

    if ( _switchLightsRELS.valid() )
    {
        if ( Data::get()->cgi.airport.lightsRELS )
            _switchLightsRELS->setAllChildrenOn();
        else
            _switchLightsRELS->setAllChildrenOff();
    }

    if ( _switchLightsTDZL.valid() )
    {
        if ( Data::get()->cgi.airport.lightsTDZL )
            _switchLightsTDZL->setAllChildrenOn();
        else
            _switchLightsTDZL->setAllChildrenOff();
    }

    if ( _switchLightsTELS.valid() )
    {
        if ( Data::get()->cgi.airport.lightsTELS )
            _switchLightsTELS->setAllChildrenOn();
        else
            _switchLightsTELS->setAllChildrenOff();
    }

    if ( _switchLightsTWRL.valid() )
    {
        if ( Data::get()->cgi.airport.lightsTWRL )
            _switchLightsTWRL->setAllChildrenOn();
        else
            _switchLightsTWRL->setAllChildrenOff();
    }

    if ( _switchLightsVGSI.valid() )
    {
        if ( Data::get()->cgi.airport.lightsVGSI )
            _switchLightsVGSI->setAllChildrenOn();
        else
            _switchLightsVGSI->setAllChildrenOff();
    }

    if ( _switchGatesRwy18.valid() )
    {
        if ( Data::get()->cgi.airport.gatesRwy18 )
            _switchGatesRwy18->setAllChildrenOn();
        else
            _switchGatesRwy18->setAllChildrenOff();
    }

    if ( _switchGatesRwy36.valid() )
    {
        if ( Data::get()->cgi.airport.gatesRwy36 )
            _switchGatesRwy36->setAllChildrenOn();
        else
            _switchGatesRwy36->setAllChildrenOff();
    }
}
