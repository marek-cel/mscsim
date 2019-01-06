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

#include <cgi/cgi_FogScene.h>

#include <osg/Fog>

#include <cgi/cgi_Color.h>
#include <cgi/cgi_Defines.h>

#include <cgi/cgi_Ownship.h>
#include <cgi/cgi_Scenery.h>
#include <cgi/cgi_SkyDome.h>

#include <Data.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

osg::Vec4 FogScene::getFogColor( float sun_elev, float visibility )
{
    int number = getFogNumber( sun_elev );

    if ( visibility < CGI_FOG_LIMIT )
    {
        return osg::Vec4( Color::fog[ number ], 1.0 );

    }
    else
    {
        return osg::Vec4( Color::sky[ number ], 1.0 );
    }
}

////////////////////////////////////////////////////////////////////////////////

int FogScene::getFogNumber( float sun_elev )
{
    int number = 0;

    if ( sun_elev >= -10.0f )
    {
        if ( sun_elev < 10.0f )
        {
            number = floor( 8 * ( sun_elev + 10.0f ) / 20.0f + 0.5f );
            number = std::min( (short)std::max( (short)number, (short)0 ), (short)8 );
        }
        else
        {
            number = 8;
        }
    }

    return number;
}

////////////////////////////////////////////////////////////////////////////////

FogScene::FogScene( Module *parent ) :
    Module( parent )
{
    m_fog = new osg::Fog();

    osg::ref_ptr<osg::StateSet> stateSet = m_root->getOrCreateStateSet();

    m_fog->setMode( osg::Fog::LINEAR );
    m_fog->setDensity( 0.5f );
    m_fog->setColor( osg::Vec4( Color::fog[ 8 ], 1.0 ) );
    m_fog->setStart( 0.0f );
    m_fog->setEnd( 0.9f * CGI_SKYDOME_RADIUS );

    stateSet->setAttributeAndModes( m_fog.get(), osg::StateAttribute::ON );
    stateSet->setMode( GL_FOG, osg::StateAttribute::ON );

    addChild( new Ownship( this ) );
    addChild( new Scenery( this ) );
}

////////////////////////////////////////////////////////////////////////////////

FogScene::~FogScene() {}

////////////////////////////////////////////////////////////////////////////////

void FogScene::update()
{
    /////////////////
    Module::update();
    /////////////////

    float visibility = Data::get()->environment.visibility;
    visibility = std::min( std::max( visibility, 1.0f ), 0.9f * CGI_SKYDOME_RADIUS );

    osg::ref_ptr<osg::StateSet> stateSet = m_root->getOrCreateStateSet();

    float elevation_deg = osg::RadiansToDegrees( Data::get()->skyDome.sunElev );

    m_fog->setMode( osg::Fog::LINEAR );
    m_fog->setDensity( 0.5f );
    m_fog->setColor( getFogColor( elevation_deg, visibility ) );
    m_fog->setStart( 0.0f );
    m_fog->setEnd( visibility );

    stateSet->setAttributeAndModes( m_fog.get(), osg::StateAttribute::ON );
    stateSet->setMode( GL_FOG, osg::StateAttribute::ON );
}
