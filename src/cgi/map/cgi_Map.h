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
#ifndef CGI_MAP_H
#define CGI_MAP_H

////////////////////////////////////////////////////////////////////////////////

#include <cgi/cgi_Module.h>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

class Icons;
class Layers;
class Traces;

/**
 * @brief Map scene root class.
 * @see Annex 4 to the Convention on International Civil Aviation - Aeronautical Charts
 */
class Map : public Module
{
public:

    static const osg::Vec3 _colorOceans;
    static const osg::Vec3 _colorLandmass;
    static const osg::Vec3 _colorCoastline;
    static const osg::Vec3 _colorCrops;
    static const osg::Vec3 _colorGrassland;
    static const osg::Vec3 _colorWoodland;
    static const osg::Vec3 _colorBuiltup;
    static const osg::Vec3 _colorRailroads;
    static const osg::Vec3 _colorRoads;
    static const osg::Vec3 _colorAirports;
    static const osg::Vec3 _colorWaterInland;
    static const osg::Vec3 _colorWaterCourse;
    static const osg::Vec3 _colorBorders;
    static const osg::Vec3 _colorAeroData;

    static const double _zOceans;
    static const double _zLandmass;
    static const double _zCoastline;
    static const double _zCrops;
    static const double _zGrassland;
    static const double _zWoodland;
    static const double _zBuiltup;
    static const double _zRailroads;
    static const double _zRoads;
    static const double _zAirports;
    static const double _zWaterCourse;
    static const double _zWaterInland;
    static const double _zSatellite;
    static const double _zBorders;

    static const double _zAerodromes;
    static const double _zNavaids;
    static const double _zTraces;
    static const double _zSpeedLeader;
    static const double _zOwnship;

    /** Constructor. */
    Map( const Module *parent = NULLPTR );

    /** Destructor. */
    virtual ~Map();

    void resetTraces();

    /** */
    void setScale( double scale );

    void setVisibilityCrops     ( bool visible );
    void setVisibilityGrassland ( bool visible );
    void setVisibilityWoodland  ( bool visible );
    void setVisibilityBuiltup   ( bool visible );
    void setVisibilityRailroads ( bool visible );
    void setVisibilityRoads     ( bool visible );
    void setVisibilityAirports  ( bool visible );
    void setVisibilitySatellite ( bool visible );
    void setVisibilityBorders   ( bool visible );
    void setVisibilityTraces    ( bool visible );

private:

    Icons  *_icons;
    Layers *_layers;
    Traces *_traces;
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_MAP_H
