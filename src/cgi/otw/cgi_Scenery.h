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
#ifndef CGI_SCENERY_H
#define CGI_SCENERY_H

////////////////////////////////////////////////////////////////////////////////

#include <osgSim/OverlayNode>

#include <fdm/xml/fdm_XmlNode.h>

#include <cgi/cgi_Module.h>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief Scenery class.
 */
class Scenery : public Module
{
public:

    /** @brief Constructor. */
    Scenery( const Module *parent = NULLPTR );

    /** @brief Destructor. */
    virtual ~Scenery();

    /** @brief Adds child to the module. */
    void addChild( Module *child );

    /** @brief Updates scenery. */
    void update();

    void setShadow( const char *shadowFile );

private:

    osg::ref_ptr<osg::PositionAttitudeTransform> _patShadow;

    osg::ref_ptr<osg::PositionAttitudeTransform> _patMasterPos;
    osg::ref_ptr<osg::PositionAttitudeTransform> _patMasterAtt;

    osg::ref_ptr<osg::PositionAttitudeTransform> _patSceneryPos;
    osg::ref_ptr<osg::PositionAttitudeTransform> _patSceneryAtt;

    osg::ref_ptr<osg::Geode> _geodeShadow;

    osg::ref_ptr<osgSim::OverlayNode> _on;

    void createShadow();

    void readAirports( const fdm::XmlNode &node );
    void readLandmarks( const fdm::XmlNode &node );

    void readTerrain( const fdm::XmlNode &node );
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_SCENERY_H
