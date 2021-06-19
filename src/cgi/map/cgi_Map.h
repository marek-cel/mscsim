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

    static const osg::Vec3 _colorOceans;        ///< color
    static const osg::Vec3 _colorLandmass;      ///< color
    static const osg::Vec3 _colorCoastline;     ///< color
    static const osg::Vec3 _colorCrops;         ///< color
    static const osg::Vec3 _colorGrassland;     ///< color
    static const osg::Vec3 _colorWoodland;      ///< color
    static const osg::Vec3 _colorBuiltup;       ///< color
    static const osg::Vec3 _colorWaterInland;   ///< color
    static const osg::Vec3 _colorWaterCourse;   ///< color
    static const osg::Vec3 _colorAirports;      ///< color
    static const osg::Vec3 _colorRailroads;     ///< color
    static const osg::Vec3 _colorRoads;         ///< color
    static const osg::Vec3 _colorBorders;       ///< color
    static const osg::Vec3 _colorAeroData;      ///< color

    static const double _zOceans;               ///< z-ccordinate
    static const double _zLandmass;             ///< z-ccordinate
    static const double _zCrops;                ///< z-ccordinate
    static const double _zGrassland;            ///< z-ccordinate
    static const double _zWoodland;             ///< z-ccordinate
    static const double _zBuiltup;              ///< z-ccordinate
    static const double _zWaterCourse;          ///< z-ccordinate
    static const double _zWaterInland;          ///< z-ccordinate
    static const double _zSatellite;            ///< z-ccordinate
    static const double _zAirports;             ///< z-ccordinate
    static const double _zRailroads;            ///< z-ccordinate
    static const double _zRoads;                ///< z-ccordinate
    static const double _zBorders;              ///< z-ccordinate
    static const double _zCoastline;            ///< z-ccordinate

    static const double _zAerodromes;           ///< z-ccordinate
    static const double _zNavaids;              ///< z-ccordinate
    static const double _zTraces;               ///< z-ccordinate
    static const double _zSpeedLeader;          ///< z-ccordinate
    static const double _zOwnship;              ///< z-ccordinate

    /** @brief Constructor. */
    Map( const Module *parent = NULLPTR );

    /** @brief Destructor. */
    virtual ~Map();

    void resetTraces();

    /** */
    void setScale( double scale );

    void setVisibilityCrops     ( bool visible );
    void setVisibilityGrassland ( bool visible );
    void setVisibilityWoodland  ( bool visible );
    void setVisibilityBuiltup   ( bool visible );
    void setVisibilityInWaters  ( bool visible );
    void setVisibilitySatellite ( bool visible );
    void setVisibilityAirports  ( bool visible );
    void setVisibilityRailroads ( bool visible );
    void setVisibilityRoads     ( bool visible );
    void setVisibilityBorders   ( bool visible );
    void setVisibilityTraces    ( bool visible );

private:

    Icons  *_icons;         ///< icons
    Layers *_layers;        ///< layers
    Traces *_traces;        ///< traces
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_MAP_H
