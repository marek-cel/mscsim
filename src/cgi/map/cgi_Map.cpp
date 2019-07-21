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

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

// DCS World
//const osg::Vec3 Map::colorOceans    = osg::Vec3( 0.49, 0.73, 0.78 );
//const osg::Vec3 Map::colorLandmass  = osg::Vec3( 0.40, 0.61, 0.42 );
//const osg::Vec3 Map::colorWoodland  = osg::Vec3( 0.35, 0.54, 0.36 );
//const osg::Vec3 Map::colorBuiltup   = osg::Vec3( 0.78, 0.51, 0.00 );
//const osg::Vec3 Map::colorRailroads = osg::Vec3( 0.11, 0.11, 0.11 );
//const osg::Vec3 Map::colorRoads     = osg::Vec3( 0.75, 0.00, 0.00 );
//const osg::Vec3 Map::colorAirports  = osg::Vec3( 0.78, 0.76, 0.68 );
//const osg::Vec3 Map::colorWater     = Map::colorOceans;

// Google Maps
//const osg::Vec3 Map::colorOceans    = osg::Vec3( 0.67, 0.85, 1.00 );
//const osg::Vec3 Map::colorLandmass  = osg::Vec3( 0.96, 0.96, 0.96 );
//const osg::Vec3 Map::colorWoodland  = osg::Vec3( 0.72, 0.84, 0.71 );
//const osg::Vec3 Map::colorBuiltup   = osg::Vec3( 0.91, 0.91, 0.91 );
//const osg::Vec3 Map::colorRailroads = osg::Vec3( 0.85, 0.85, 0.85 );
//const osg::Vec3 Map::colorRoads     = osg::Vec3( 0.98, 0.70, 0.61 );
//const osg::Vec3 Map::colorAirports  = osg::Vec3( 0.91, 0.91, 0.91 );
//const osg::Vec3 Map::colorWater     = Map::colorOceans;

// OpenStreetMap
const osg::Vec3 Map::colorOceans    = osg::Vec3( 0.68, 0.87, 1.00 );
const osg::Vec3 Map::colorLandmass  = osg::Vec3( 0.95, 0.94, 0.91 );
const osg::Vec3 Map::colorWoodland  = osg::Vec3( 0.82, 0.91, 0.82 );
const osg::Vec3 Map::colorBuiltup   = osg::Vec3( 0.80, 0.80, 0.80 );
const osg::Vec3 Map::colorRailroads = osg::Vec3( 0.60, 0.60, 0.60 );
const osg::Vec3 Map::colorRoads     = osg::Vec3( 0.81, 0.63, 0.32 );
const osg::Vec3 Map::colorAirports  = osg::Vec3( 0.74, 0.74, 0.80 );
const osg::Vec3 Map::colorWater     = Map::colorOceans;

const double Map::zOceans    = -18.0;
const double Map::zLandmass  = -17.0;
const double Map::zWoodland  = -16.0;
const double Map::zBuiltup   = -15.0;
const double Map::zRailroads = -14.0;
const double Map::zRoads     = -13.0;
const double Map::zAirports  = -12.0;
const double Map::zWater     = -11.0;

const double Map::zSpeedLeader = -2.0;
const double Map::zIconsFill   = -1.0;

////////////////////////////////////////////////////////////////////////////////

Map::Map( Module *parent ) :
    Module( parent )
{
    osg::ref_ptr<osg::StateSet> stateSet = m_root->getOrCreateStateSet();

    stateSet->setMode( GL_RESCALE_NORMAL , osg::StateAttribute::ON  );
    stateSet->setMode( GL_LIGHTING       , osg::StateAttribute::OFF );
    stateSet->setMode( GL_LIGHT0         , osg::StateAttribute::ON  );
    stateSet->setMode( GL_BLEND          , osg::StateAttribute::ON  );
    stateSet->setMode( GL_ALPHA_TEST     , osg::StateAttribute::ON  );
    stateSet->setMode( GL_DEPTH_TEST     , osg::StateAttribute::ON  );
    stateSet->setMode( GL_DITHER         , osg::StateAttribute::OFF );
    stateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
    stateSet->setRenderBinDetails( CGI_DEPTH_SORTED_BIN_WORLD, "RenderBin" );

    addChild( new Icons( this ) );
    addChild( new Layers( this ) );
}

////////////////////////////////////////////////////////////////////////////////

Map::~Map() {}

////////////////////////////////////////////////////////////////////////////////

void Map::setScale( double scale )
{
    for ( List::iterator it = m_children.begin(); it != m_children.end(); it++ )
    {
        Icons *icons = dynamic_cast< Icons* >( *it );

        if ( icons )
        {
            icons->setScale( scale );
        }
    }
}
