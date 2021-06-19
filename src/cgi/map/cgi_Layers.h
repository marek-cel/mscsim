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
#ifndef CGI_LAYERS_H
#define CGI_LAYERS_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/PositionAttitudeTransform>
#include <osg/Switch>

#include <fdm/xml/fdm_XmlNode.h>

#include <cgi/cgi_Module.h>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/** Layers class. */
class Layers : public Module
{
public:

    /** @brief Constructor. */
    Layers( const Module *parent = NULLPTR );

    /** @brief Destructor. */
    virtual ~Layers();

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

private:

    osg::ref_ptr<osg::Switch> _switchCrops;
    osg::ref_ptr<osg::Switch> _switchGrassland;
    osg::ref_ptr<osg::Switch> _switchWoodland;
    osg::ref_ptr<osg::Switch> _switchBuiltup;
    osg::ref_ptr<osg::Switch> _switchInWaters;
    osg::ref_ptr<osg::Switch> _switchSatellite;
    osg::ref_ptr<osg::Switch> _switchAirports;
    osg::ref_ptr<osg::Switch> _switchRailroads;
    osg::ref_ptr<osg::Switch> _switchRoads;
    osg::ref_ptr<osg::Switch> _switchBorders;
    osg::ref_ptr<osg::Switch> _switchCoastline;

    osg::ref_ptr<osg::PositionAttitudeTransform> _oceans;
    osg::ref_ptr<osg::PositionAttitudeTransform> _landmass;
    osg::ref_ptr<osg::PositionAttitudeTransform> _crops;
    osg::ref_ptr<osg::PositionAttitudeTransform> _grassland;
    osg::ref_ptr<osg::PositionAttitudeTransform> _woodland;
    osg::ref_ptr<osg::PositionAttitudeTransform> _builtup;
    osg::ref_ptr<osg::PositionAttitudeTransform> _water_course;
    osg::ref_ptr<osg::PositionAttitudeTransform> _water_inland;
    osg::ref_ptr<osg::PositionAttitudeTransform> _satellite;
    osg::ref_ptr<osg::PositionAttitudeTransform> _airports;
    osg::ref_ptr<osg::PositionAttitudeTransform> _railroads;
    osg::ref_ptr<osg::PositionAttitudeTransform> _roads;
    osg::ref_ptr<osg::PositionAttitudeTransform> _borders;
    osg::ref_ptr<osg::PositionAttitudeTransform> _coastline;

    void createOcean();

    void initLayer( osg::Node* layer, osg::Vec3 color, float width = 1.0f );

    void readLayers();
    void readLayer( const fdm::XmlNode &node, osg::Group *parent );
    void readLayerSatellite( const fdm::XmlNode &node, osg::Group *parent );

    void readSatelliteImage( osg::Group *parent, const char *file );
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_LAYERS_H
