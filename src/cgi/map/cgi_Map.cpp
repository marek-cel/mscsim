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

#include <cgi/map/cgi_Map.h>

#include <cgi/cgi_Defines.h>

#include <cgi/map/cgi_Icons.h>
#include <cgi/map/cgi_Layers.h>
#include <cgi/map/cgi_Traces.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

// DCS World
//const osg::Vec3 Map::colorOceans      = osg::Vec3( 0.49, 0.73, 0.78 );
//const osg::Vec3 Map::colorLandmass    = osg::Vec3( 0.40, 0.61, 0.42 );
//const osg::Vec3 Map::colorCoastline   = osg::Vec3( 0.01, 0.01, 0.01 );
//const osg::Vec3 Map::colorCrops       = osg::Vec3( 0.00, 0.00, 0.00 );
//const osg::Vec3 Map::colorGrassland   = osg::Vec3( 0.00, 0.00, 0.00 );
//const osg::Vec3 Map::colorWoodland    = osg::Vec3( 0.35, 0.54, 0.36 );
//const osg::Vec3 Map::colorBuiltup     = osg::Vec3( 0.78, 0.51, 0.00 );
//const osg::Vec3 Map::colorRailroads   = osg::Vec3( 0.11, 0.11, 0.11 );
//const osg::Vec3 Map::colorRoads       = osg::Vec3( 0.75, 0.00, 0.00 );
//const osg::Vec3 Map::colorAirports    = osg::Vec3( 0.78, 0.76, 0.68 );
//const osg::Vec3 Map::colorWaterCourse = Map::colorOceans;
//const osg::Vec3 Map::colorWaterInland = Map::colorOceans;
//const osg::Vec3 Map::colorBorders     = osg::Vec3( 0.00, 0.00, 0.00 );
//const osg::Vec3 Map::colorAeroData    = osg::Vec3( 0.00, 0.00, 0.00 );

// Google Maps
//const osg::Vec3 Map::colorOceans      = osg::Vec3( 0.67, 0.85, 1.00 );
//const osg::Vec3 Map::colorLandmass    = osg::Vec3( 0.96, 0.96, 0.96 );
//const osg::Vec3 Map::colorCoastline   = osg::Vec3( 0.01, 0.01, 0.01 );
//const osg::Vec3 Map::colorCrops       = osg::Vec3( 0.00, 0.00, 0.00 );
//const osg::Vec3 Map::colorGrassland   = osg::Vec3( 0.93, 0.97, 0.92 );
//const osg::Vec3 Map::colorWoodland    = osg::Vec3( 0.72, 0.84, 0.71 );
//const osg::Vec3 Map::colorBuiltup     = osg::Vec3( 0.91, 0.91, 0.91 );
//const osg::Vec3 Map::colorRailroads   = osg::Vec3( 0.85, 0.85, 0.85 );
//const osg::Vec3 Map::colorRoads       = osg::Vec3( 0.98, 0.70, 0.61 );
//const osg::Vec3 Map::colorAirports    = osg::Vec3( 0.91, 0.91, 0.91 );
//const osg::Vec3 Map::colorWaterCourse = Map::colorOceans;
//const osg::Vec3 Map::colorWaterInland = Map::colorOceans;
//const osg::Vec3 Map::colorBorders     = osg::Vec3( 0.48, 0.49, 0.49 );
//const osg::Vec3 Map::colorAeroData    = osg::Vec3( 0.00, 0.00, 0.00 );

// OpenStreetMap
//const osg::Vec3 Map::colorOceans      = osg::Vec3( 0.67, 0.83, 0.87 );
//const osg::Vec3 Map::colorLandmass    = osg::Vec3( 0.95, 0.94, 0.91 );
//const osg::Vec3 Map::colorCoastline   = osg::Vec3( 0.51, 0.75, 0.82 );
//const osg::Vec3 Map::colorCrops       = osg::Vec3( 0.95, 0.95, 0.87 );
//const osg::Vec3 Map::colorGrassland   = osg::Vec3( 0.88, 0.95, 0.81 );
//const osg::Vec3 Map::colorWoodland    = osg::Vec3( 0.83, 0.91, 0.80 );
//const osg::Vec3 Map::colorBuiltup     = osg::Vec3( 0.87, 0.87, 0.87 );
//const osg::Vec3 Map::colorRailroads   = osg::Vec3( 0.60, 0.60, 0.60 );
//const osg::Vec3 Map::colorRoads       = osg::Vec3( 0.81, 0.63, 0.32 );
//const osg::Vec3 Map::colorAirports    = osg::Vec3( 0.74, 0.74, 0.80 );
//const osg::Vec3 Map::colorWaterCourse = Map::colorOceans;
//const osg::Vec3 Map::colorWaterInland = Map::colorOceans;
//const osg::Vec3 Map::colorBorders     = osg::Vec3( 0.75, 0.50, 0.75 );
//const osg::Vec3 Map::colorAeroData    = osg::Vec3( 0.00, 0.00, 0.00 );

// PANSA
//const osg::Vec3 Map::colorOceans      = osg::Vec3( 0.84, 0.91, 0.97 );
//const osg::Vec3 Map::colorLandmass    = osg::Vec3( 1.00, 1.00, 1.00 );
//const osg::Vec3 Map::colorCoastline   = osg::Vec3( 0.01, 0.01, 0.01 );
//const osg::Vec3 Map::colorCrops       = osg::Vec3( 1.00, 1.00, 1.00 );
//const osg::Vec3 Map::colorGrassland   = osg::Vec3( 0.93, 0.97, 0.92 );
//const osg::Vec3 Map::colorWoodland    = osg::Vec3( 0.79, 0.92, 0.79 );
//const osg::Vec3 Map::colorBuiltup     = osg::Vec3( 0.84, 0.83, 0.84 );
//const osg::Vec3 Map::colorRailroads   = osg::Vec3( 0.51, 0.51, 0.51 );
//const osg::Vec3 Map::colorRoads       = osg::Vec3( 0.67, 0.66, 0.66 );
//const osg::Vec3 Map::colorAirports    = osg::Vec3( 0.47, 0.47, 0.19 );
//const osg::Vec3 Map::colorWaterCourse = Map::colorOceans;
//const osg::Vec3 Map::colorWaterInland = Map::colorOceans;
//const osg::Vec3 Map::colorBorders     = osg::Vec3( 0.00, 0.00, 0.00 );
//const osg::Vec3 Map::colorAeroData    = osg::Vec3( 0.00, 0.00, 0.00 );

// ICAO
const osg::Vec3 Map::colorOceans      = osg::Vec3( 0.73, 0.77, 0.82 );
const osg::Vec3 Map::colorLandmass    = osg::Vec3( 1.00, 1.00, 1.00 );
const osg::Vec3 Map::colorCoastline   = osg::Vec3( 0.11, 0.20, 0.39 );
const osg::Vec3 Map::colorCrops       = osg::Vec3( 1.00, 1.00, 1.00 );
const osg::Vec3 Map::colorGrassland   = osg::Vec3( 1.00, 1.00, 1.00 );
const osg::Vec3 Map::colorWoodland    = osg::Vec3( 0.64, 0.60, 0.20 );
const osg::Vec3 Map::colorBuiltup     = osg::Vec3( 0.99, 0.92, 0.00 );
const osg::Vec3 Map::colorRailroads   = osg::Vec3( 0.22, 0.15, 0.13 );
const osg::Vec3 Map::colorRoads       = osg::Vec3( 0.94, 0.22, 0.14 );
const osg::Vec3 Map::colorAirports    = osg::Vec3( 0.47, 0.15, 0.11 );
const osg::Vec3 Map::colorWaterCourse = Map::colorCoastline;
const osg::Vec3 Map::colorWaterInland = Map::colorOceans;
const osg::Vec3 Map::colorBorders     = osg::Vec3( 0.22, 0.15, 0.13 );
const osg::Vec3 Map::colorAeroData    = osg::Vec3( 0.18, 0.19, 0.50 );

const double Map::zOceans      = -24.0;
const double Map::zLandmass    = -23.0;
const double Map::zCoastline   = -22.0;
const double Map::zCrops       = -21.0;
const double Map::zGrassland   = -20.0;
const double Map::zWoodland    = -19.0;
const double Map::zBuiltup     = -18.0;
const double Map::zRailroads   = -17.0;
const double Map::zRoads       = -16.0;
const double Map::zWaterCourse = -15.0;
const double Map::zWaterInland = -14.0;
const double Map::zSatellite   = -13.0;
const double Map::zAirports    = -12.0;
const double Map::zBorders     = -11.0;

const double Map::zAerodromes  = -3.0;
const double Map::zTraces      = -2.0;
const double Map::zSpeedLeader = -2.0;
const double Map::zOwnship     = -1.0;

////////////////////////////////////////////////////////////////////////////////

Map::Map( Module *parent ) :
    Module( parent ),

    m_icons  ( 0 ),
    m_layers ( 0 ),
    m_traces ( 0 )
{
    osg::ref_ptr<osg::StateSet> stateSet = m_root->getOrCreateStateSet();

    stateSet->setMode( GL_RESCALE_NORMAL , osg::StateAttribute::ON  );
    stateSet->setMode( GL_LIGHTING       , osg::StateAttribute::OFF );
    stateSet->setMode( GL_LIGHT0         , osg::StateAttribute::OFF );
    stateSet->setMode( GL_BLEND          , osg::StateAttribute::ON  );
    stateSet->setMode( GL_ALPHA_TEST     , osg::StateAttribute::ON  );
    stateSet->setMode( GL_DEPTH_TEST     , osg::StateAttribute::ON  );
    stateSet->setMode( GL_DITHER         , osg::StateAttribute::OFF );
    stateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
    stateSet->setRenderBinDetails( CGI_DEPTH_SORTED_BIN_MAP, "RenderBin" );

    addChild( m_icons  = new Icons  ( this ) );
    addChild( m_layers = new Layers ( this ) );
    addChild( m_traces = new Traces ( this ) );
}

////////////////////////////////////////////////////////////////////////////////

Map::~Map() {}

////////////////////////////////////////////////////////////////////////////////

void Map::resetTraces()
{
    m_traces->reset();
}

////////////////////////////////////////////////////////////////////////////////

void Map::setScale( double scale )
{
    m_icons->setScale( scale );
}

////////////////////////////////////////////////////////////////////////////////

void Map::setVisibilityCrops( bool visible )
{
    m_layers->setVisibilityCrops( visible );
}

////////////////////////////////////////////////////////////////////////////////

void Map::setVisibilityGrassland( bool visible )
{
    m_layers->setVisibilityGrassland( visible );
}

////////////////////////////////////////////////////////////////////////////////

void Map::setVisibilityWoodland( bool visible )
{
    m_layers->setVisibilityWoodland( visible );
}

////////////////////////////////////////////////////////////////////////////////

void Map::setVisibilityBuiltup( bool visible )
{
    m_layers->setVisibilityBuiltup( visible );
}

////////////////////////////////////////////////////////////////////////////////

void Map::setVisibilityRailroads( bool visible )
{
    m_layers->setVisibilityRailroads( visible );
}

////////////////////////////////////////////////////////////////////////////////

void Map::setVisibilityRoads( bool visible )
{
    m_layers->setVisibilityRoads( visible );
}

////////////////////////////////////////////////////////////////////////////////

void Map::setVisibilityAirports( bool visible )
{
    m_layers->setVisibilityAirports( visible );
}

////////////////////////////////////////////////////////////////////////////////

void Map::setVisibilitySatellite( bool visible )
{
    m_layers->setVisibilitySatellite( visible );
}

////////////////////////////////////////////////////////////////////////////////

void Map::setVisibilityBorders( bool visible )
{
    m_layers->setVisibilityBorders( visible );
}

////////////////////////////////////////////////////////////////////////////////

void Map::setVisibilityTraces( bool visible )
{
    m_traces->setVisibility( visible );
}
