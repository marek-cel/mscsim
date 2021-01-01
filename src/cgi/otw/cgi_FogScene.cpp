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

#include <cgi/otw/cgi_FogScene.h>

#include <osg/Fog>

#include <cgi/cgi_Colors.h>
#include <cgi/cgi_Defines.h>

#include <cgi/otw/cgi_Clouds.h>
#include <cgi/otw/cgi_Ownship.h>
#include <cgi/otw/cgi_Scenery.h>
#include <cgi/otw/cgi_SkyDome.h>

#include <Data.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

osg::Vec4 FogScene::getFogColor( float sun_elev, float visibility )
{
    int number = getFogNumber( sun_elev );

    if ( visibility < CGI_FOG_LIMIT )
    {
        return osg::Vec4( Colors::_fog[ number ], 1.0 );

    }
    else
    {
        return osg::Vec4( Colors::_sky[ number ], 1.0 );
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

FogScene::FogScene( const Module *parent ) :
    Module( parent )
{
#   ifndef SIM_TEST_WORLD
    _fog = new osg::Fog();

    osg::ref_ptr<osg::StateSet> stateSet = _root->getOrCreateStateSet();

    _fog->setMode( osg::Fog::LINEAR );
    _fog->setDensity( 0.5f );
    _fog->setColor( osg::Vec4( Colors::_fog[ 8 ], 1.0 ) );
    _fog->setStart( 0.0f );
    _fog->setEnd( 0.9f * CGI_SKYDOME_RADIUS );

    stateSet->setAttributeAndModes( _fog.get(), osg::StateAttribute::ON );
    stateSet->setMode( GL_FOG, osg::StateAttribute::ON );
#   endif // SIM_TEST_WORLD

    Clouds  *clouds  = new Clouds  ( this );
    Scenery *scenery = new Scenery ( this );
    Ownship *ownship = new Ownship ( this, scenery );

    addChild( clouds  );
    addChild( ownship );
    addChild( scenery );
}

////////////////////////////////////////////////////////////////////////////////

FogScene::~FogScene() {}

////////////////////////////////////////////////////////////////////////////////

void FogScene::update()
{
    /////////////////
    Module::update();
    /////////////////

#   ifndef SIM_TEST_WORLD
    float visibility = Data::get()->cgi.environment.visibility;
    visibility = std::min( std::max( visibility, 1.0f ), 0.9f * CGI_SKYDOME_RADIUS );

    osg::ref_ptr<osg::StateSet> stateSet = _root->getOrCreateStateSet();

    float elevation_deg = osg::RadiansToDegrees( Data::get()->cgi.skyDome.sunElev );

    _fog->setMode( osg::Fog::LINEAR );
    _fog->setDensity( 0.5f );
    _fog->setColor( getFogColor( elevation_deg, visibility ) );
    _fog->setStart( 0.0f );
    _fog->setEnd( visibility );

    stateSet->setAttributeAndModes( _fog.get(), osg::StateAttribute::ON );
    stateSet->setMode( GL_FOG, osg::StateAttribute::ON );
#   endif // SIM_TEST_WORLD
}
