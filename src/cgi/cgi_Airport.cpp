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

#include <cgi/cgi_Airport.h>

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
    m_pat = new osg::PositionAttitudeTransform();
    m_root->addChild( m_pat.get() );

    osg::ref_ptr<osg::Node> airportNode = Models::get( file );

    if ( airportNode.valid() )
    {
        m_pat->addChild( airportNode.get() );

        // position
        WGS84 wgs( lat, lon, alt );

        m_pat->setPosition( wgs.getPosition() );
        m_pat->setAttitude( wgs.getAttitude() );

        m_switchLightsHELI = dynamic_cast<osg::Switch*>( FindNode::findFirst( airportNode, "HELI" ) );
        m_switchLightsRALS = dynamic_cast<osg::Switch*>( FindNode::findFirst( airportNode, "RALS" ) );
        m_switchLightsRCLS = dynamic_cast<osg::Switch*>( FindNode::findFirst( airportNode, "RCLS" ) );
        m_switchLightsRELS = dynamic_cast<osg::Switch*>( FindNode::findFirst( airportNode, "RELS" ) );
        m_switchLightsTDZL = dynamic_cast<osg::Switch*>( FindNode::findFirst( airportNode, "TDZL" ) );
        m_switchLightsTELS = dynamic_cast<osg::Switch*>( FindNode::findFirst( airportNode, "TELS" ) );
        m_switchLightsTWRL = dynamic_cast<osg::Switch*>( FindNode::findFirst( airportNode, "TWRL" ) );
        m_switchLightsVGSI = dynamic_cast<osg::Switch*>( FindNode::findFirst( airportNode, "VGSI" ) );
        m_switchGatesRwy18 = dynamic_cast<osg::Switch*>( FindNode::findFirst( airportNode, "ApproachGates18" ) );
        m_switchGatesRwy36 = dynamic_cast<osg::Switch*>( FindNode::findFirst( airportNode, "ApproachGates36" ) );
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

    if ( m_switchLightsHELI.valid() )
    {
        if ( Data::get()->airport.lightsHELI )
            m_switchLightsHELI->setAllChildrenOn();
        else
            m_switchLightsHELI->setAllChildrenOff();
    }

    if ( m_switchLightsRALS.valid() )
    {
        if ( Data::get()->airport.lightsRALS )
            m_switchLightsRALS->setAllChildrenOn();
        else
            m_switchLightsRALS->setAllChildrenOff();
    }

    if ( m_switchLightsRCLS.valid() )
    {
        if ( Data::get()->airport.lightsRCLS )
            m_switchLightsRCLS->setAllChildrenOn();
        else
            m_switchLightsRCLS->setAllChildrenOff();
    }

    if ( m_switchLightsRELS.valid() )
    {
        if ( Data::get()->airport.lightsRELS )
            m_switchLightsRELS->setAllChildrenOn();
        else
            m_switchLightsRELS->setAllChildrenOff();
    }

    if ( m_switchLightsTDZL.valid() )
    {
        if ( Data::get()->airport.lightsTDZL )
            m_switchLightsTDZL->setAllChildrenOn();
        else
            m_switchLightsTDZL->setAllChildrenOff();
    }

    if ( m_switchLightsTELS.valid() )
    {
        if ( Data::get()->airport.lightsTELS )
            m_switchLightsTELS->setAllChildrenOn();
        else
            m_switchLightsTELS->setAllChildrenOff();
    }

    if ( m_switchLightsTWRL.valid() )
    {
        if ( Data::get()->airport.lightsTWRL )
            m_switchLightsTWRL->setAllChildrenOn();
        else
            m_switchLightsTWRL->setAllChildrenOff();
    }

    if ( m_switchLightsVGSI.valid() )
    {
        if ( Data::get()->airport.lightsVGSI )
            m_switchLightsVGSI->setAllChildrenOn();
        else
            m_switchLightsVGSI->setAllChildrenOff();
    }

    if ( m_switchGatesRwy18.valid() )
    {
        if ( Data::get()->airport.gatesRwy18 )
            m_switchGatesRwy18->setAllChildrenOn();
        else
            m_switchGatesRwy18->setAllChildrenOff();
    }

    if ( m_switchGatesRwy36.valid() )
    {
        if ( Data::get()->airport.gatesRwy36 )
            m_switchGatesRwy36->setAllChildrenOn();
        else
            m_switchGatesRwy36->setAllChildrenOff();
    }
}
