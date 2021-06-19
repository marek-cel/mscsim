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

#include <cgi/map/cgi_Map.h>

#include <cgi/cgi_Defines.h>

#include <cgi/map/cgi_Icons.h>
#include <cgi/map/cgi_Layers.h>
#include <cgi/map/cgi_Traces.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

// DCS World
//const osg::Vec3 Map::_colorOceans      = osg::Vec3( 0.49f, 0.73f, 0.78f );
//const osg::Vec3 Map::_colorLandmass    = osg::Vec3( 0.40f, 0.61f, 0.42f );
//const osg::Vec3 Map::_colorCoastline   = osg::Vec3( 0.01f, 0.01f, 0.01f );
//const osg::Vec3 Map::_colorCrops       = osg::Vec3( 0.00f, 0.00f, 0.00f );
//const osg::Vec3 Map::_colorGrassland   = osg::Vec3( 0.00f, 0.00f, 0.00f );
//const osg::Vec3 Map::_colorWoodland    = osg::Vec3( 0.35f, 0.54f, 0.36f );
//const osg::Vec3 Map::_colorBuiltup     = osg::Vec3( 0.78f, 0.51f, 0.00f );
//const osg::Vec3 Map::_colorWaterCourse = Map::_colorOceans;
//const osg::Vec3 Map::_colorWaterInland = Map::_colorOceans;
//const osg::Vec3 Map::_colorAirports    = osg::Vec3( 0.78f, 0.76f, 0.68f );
//const osg::Vec3 Map::_colorRailroads   = osg::Vec3( 0.11f, 0.11f, 0.11f );
//const osg::Vec3 Map::_colorRoads       = osg::Vec3( 0.75f, 0.00f, 0.00f );
//const osg::Vec3 Map::_colorBorders     = osg::Vec3( 0.00f, 0.00f, 0.00f );
//const osg::Vec3 Map::_colorAeroData    = osg::Vec3( 0.00f, 0.00f, 0.00f );

// Google Maps
//const osg::Vec3 Map::_colorOceans      = osg::Vec3( 0.67f, 0.85f, 1.00f );
//const osg::Vec3 Map::_colorLandmass    = osg::Vec3( 0.96f, 0.96f, 0.96f );
//const osg::Vec3 Map::_colorCoastline   = osg::Vec3( 0.01f, 0.01f, 0.01f );
//const osg::Vec3 Map::_colorCrops       = osg::Vec3( 0.00f, 0.00f, 0.00f );
//const osg::Vec3 Map::_colorGrassland   = osg::Vec3( 0.93f, 0.97f, 0.92f );
//const osg::Vec3 Map::_colorWoodland    = osg::Vec3( 0.72f, 0.84f, 0.71f );
//const osg::Vec3 Map::_colorBuiltup     = osg::Vec3( 0.91f, 0.91f, 0.91f );
//const osg::Vec3 Map::_colorWaterCourse = Map::_colorOceans;
//const osg::Vec3 Map::_colorWaterInland = Map::_colorOceans;
//const osg::Vec3 Map::_colorAirports    = osg::Vec3( 0.91f, 0.91f, 0.91f );
//const osg::Vec3 Map::_colorRailroads   = osg::Vec3( 0.85f, 0.85f, 0.85f );
//const osg::Vec3 Map::_colorRoads       = osg::Vec3( 0.98f, 0.70f, 0.61f );
//const osg::Vec3 Map::_colorBorders     = osg::Vec3( 0.48f, 0.49f, 0.49f );
//const osg::Vec3 Map::_colorAeroData    = osg::Vec3( 0.00f, 0.00f, 0.00f );

// OpenStreetMap
//const osg::Vec3 Map::_colorOceans      = osg::Vec3( 0.67f, 0.83f, 0.87f );
//const osg::Vec3 Map::_colorLandmass    = osg::Vec3( 0.95f, 0.94f, 0.91f );
//const osg::Vec3 Map::_colorCoastline   = osg::Vec3( 0.51f, 0.75f, 0.82f );
//const osg::Vec3 Map::_colorCrops       = osg::Vec3( 0.95f, 0.95f, 0.87f );
//const osg::Vec3 Map::_colorGrassland   = osg::Vec3( 0.88f, 0.95f, 0.81f );
//const osg::Vec3 Map::_colorWoodland    = osg::Vec3( 0.83f, 0.91f, 0.80f );
//const osg::Vec3 Map::_colorBuiltup     = osg::Vec3( 0.87f, 0.87f, 0.87f );
//const osg::Vec3 Map::_colorWaterCourse = Map::_colorOceans;
//const osg::Vec3 Map::_colorWaterInland = Map::_colorOceans;
//const osg::Vec3 Map::_colorAirports    = osg::Vec3( 0.74f, 0.74f, 0.80f );
//const osg::Vec3 Map::_colorRailroads   = osg::Vec3( 0.60f, 0.60f, 0.60f );
//const osg::Vec3 Map::_colorRoads       = osg::Vec3( 0.81f, 0.63f, 0.32f );
//const osg::Vec3 Map::_colorBorders     = osg::Vec3( 0.75f, 0.50f, 0.75f );
//const osg::Vec3 Map::_colorAeroData    = osg::Vec3( 0.00f, 0.00f, 0.00f );

// PANSA
//const osg::Vec3 Map::_colorOceans      = osg::Vec3( 0.84f, 0.91f, 0.97f );
//const osg::Vec3 Map::_colorLandmass    = osg::Vec3( 1.00f, 1.00f, 1.00f );
//const osg::Vec3 Map::_colorCoastline   = osg::Vec3( 0.01f, 0.01f, 0.01f );
//const osg::Vec3 Map::_colorCrops       = osg::Vec3( 1.00f, 1.00f, 1.00f );
//const osg::Vec3 Map::_colorGrassland   = osg::Vec3( 0.93f, 0.97f, 0.92f );
//const osg::Vec3 Map::_colorWoodland    = osg::Vec3( 0.79f, 0.92f, 0.79f );
//const osg::Vec3 Map::_colorBuiltup     = osg::Vec3( 0.84f, 0.83f, 0.84f );
//const osg::Vec3 Map::_colorWaterCourse = Map::_colorOceans;
//const osg::Vec3 Map::_colorWaterInland = Map::_colorOceans;
//const osg::Vec3 Map::_colorAirports    = osg::Vec3( 0.47f, 0.47f, 0.19f );
//const osg::Vec3 Map::_colorRailroads   = osg::Vec3( 0.51f, 0.51f, 0.51f );
//const osg::Vec3 Map::_colorRoads       = osg::Vec3( 0.67f, 0.66f, 0.66f );
//const osg::Vec3 Map::_colorBorders     = osg::Vec3( 0.00f, 0.00f, 0.00f );
//const osg::Vec3 Map::_colorAeroData    = osg::Vec3( 0.00f, 0.00f, 0.00f );

// ICAO Annex 4 - Aeronautical Charts
const osg::Vec3 Map::_colorOceans      = osg::Vec3( 0.73f, 0.77f, 0.82f );
const osg::Vec3 Map::_colorLandmass    = osg::Vec3( 1.00f, 1.00f, 1.00f );
const osg::Vec3 Map::_colorCoastline   = osg::Vec3( 0.11f, 0.20f, 0.39f );
const osg::Vec3 Map::_colorCrops       = osg::Vec3( 1.00f, 1.00f, 1.00f );
const osg::Vec3 Map::_colorGrassland   = osg::Vec3( 1.00f, 1.00f, 1.00f );
const osg::Vec3 Map::_colorWoodland    = osg::Vec3( 0.64f, 0.60f, 0.20f );
const osg::Vec3 Map::_colorBuiltup     = osg::Vec3( 0.99f, 0.92f, 0.00f );
const osg::Vec3 Map::_colorWaterCourse = Map::_colorCoastline;
const osg::Vec3 Map::_colorWaterInland = Map::_colorOceans;
const osg::Vec3 Map::_colorAirports    = osg::Vec3( 0.47f, 0.15f, 0.11f );
const osg::Vec3 Map::_colorRailroads   = osg::Vec3( 0.22f, 0.15f, 0.13f );
const osg::Vec3 Map::_colorRoads       = osg::Vec3( 0.94f, 0.22f, 0.14f );
const osg::Vec3 Map::_colorBorders     = osg::Vec3( 0.22f, 0.15f, 0.13f );
const osg::Vec3 Map::_colorAeroData    = osg::Vec3( 0.18f, 0.19f, 0.50f );

const double Map::_zOceans      = -24.0;
const double Map::_zLandmass    = -23.0;
const double Map::_zCrops       = -22.0;
const double Map::_zGrassland   = -21.0;
const double Map::_zWoodland    = -20.0;
const double Map::_zBuiltup     = -19.0;
const double Map::_zWaterCourse = -18.0;
const double Map::_zWaterInland = -17.0;
const double Map::_zSatellite   = -16.0;
const double Map::_zAirports    = -15.0;
const double Map::_zRailroads   = -14.0;
const double Map::_zRoads       = -13.0;
const double Map::_zBorders     = -12.0;
const double Map::_zCoastline   = -11.0;

const double Map::_zAerodromes  = -3.0;
const double Map::_zNavaids     = -3.0;
const double Map::_zTraces      = -2.0;
const double Map::_zSpeedLeader = -2.0;
const double Map::_zOwnship     = -1.0;

////////////////////////////////////////////////////////////////////////////////

Map::Map( const Module *parent ) :
    Module( parent ),

    _icons  ( NULLPTR ),
    _layers ( NULLPTR ),
    _traces ( NULLPTR )
{
    osg::ref_ptr<osg::StateSet> stateSet = _root->getOrCreateStateSet();

    stateSet->setMode( GL_RESCALE_NORMAL , osg::StateAttribute::ON  );
    stateSet->setMode( GL_LIGHTING       , osg::StateAttribute::OFF );
    stateSet->setMode( GL_LIGHT0         , osg::StateAttribute::OFF );
    stateSet->setMode( GL_BLEND          , osg::StateAttribute::ON  );
    stateSet->setMode( GL_ALPHA_TEST     , osg::StateAttribute::ON  );
    stateSet->setMode( GL_DEPTH_TEST     , osg::StateAttribute::ON  );
    stateSet->setMode( GL_DITHER         , osg::StateAttribute::OFF );
    stateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
    stateSet->setRenderBinDetails( CGI_DEPTH_SORTED_BIN_MAP, "RenderBin" );

    addChild( _icons  = new Icons  ( this ) );
    addChild( _layers = new Layers ( this ) );
    addChild( _traces = new Traces ( this ) );
}

////////////////////////////////////////////////////////////////////////////////

Map::~Map() {}

////////////////////////////////////////////////////////////////////////////////

void Map::resetTraces()
{
    _traces->reset();
}

////////////////////////////////////////////////////////////////////////////////

void Map::setScale( double scale )
{
    _icons->setScale( scale );
}

////////////////////////////////////////////////////////////////////////////////

void Map::setVisibilityCrops( bool visible )
{
    _layers->setVisibilityCrops( visible );
}

////////////////////////////////////////////////////////////////////////////////

void Map::setVisibilityGrassland( bool visible )
{
    _layers->setVisibilityGrassland( visible );
}

////////////////////////////////////////////////////////////////////////////////

void Map::setVisibilityWoodland( bool visible )
{
    _layers->setVisibilityWoodland( visible );
}

////////////////////////////////////////////////////////////////////////////////

void Map::setVisibilityBuiltup( bool visible )
{
    _layers->setVisibilityBuiltup( visible );
}

////////////////////////////////////////////////////////////////////////////////

void Map::setVisibilityInWaters( bool visible )
{
    _layers->setVisibilityInWaters( visible );
}

////////////////////////////////////////////////////////////////////////////////

void Map::setVisibilitySatellite( bool visible )
{
    _layers->setVisibilitySatellite( visible );
}

////////////////////////////////////////////////////////////////////////////////

void Map::setVisibilityAirports( bool visible )
{
    _layers->setVisibilityAirports( visible );
}

////////////////////////////////////////////////////////////////////////////////

void Map::setVisibilityRailroads( bool visible )
{
    _layers->setVisibilityRailroads( visible );
}

////////////////////////////////////////////////////////////////////////////////

void Map::setVisibilityRoads( bool visible )
{
    _layers->setVisibilityRoads( visible );
}

////////////////////////////////////////////////////////////////////////////////

void Map::setVisibilityBorders( bool visible )
{
    _layers->setVisibilityBorders( visible );
}

////////////////////////////////////////////////////////////////////////////////

void Map::setVisibilityTraces( bool visible )
{
    _traces->setVisibility( visible );
}
