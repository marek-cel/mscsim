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
#ifndef CGI_LAYERS_H
#define CGI_LAYERS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/xml/fdm_XmlNode.h>

#include <cgi/cgi_Module.h>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/** */
class Layers : public Module
{
public:

    /** Constructor. */
    Layers( Module *parent = 0 );

    /** Destructor. */
    virtual ~Layers();

private:

    osg::ref_ptr<osg::PositionAttitudeTransform> m_oceans;
    osg::ref_ptr<osg::PositionAttitudeTransform> m_landmass;
    osg::ref_ptr<osg::PositionAttitudeTransform> m_woodland;
    osg::ref_ptr<osg::PositionAttitudeTransform> m_builtup;
    osg::ref_ptr<osg::PositionAttitudeTransform> m_railroads;
    osg::ref_ptr<osg::PositionAttitudeTransform> m_roads;
    osg::ref_ptr<osg::PositionAttitudeTransform> m_airports;
    osg::ref_ptr<osg::PositionAttitudeTransform> m_water;

    void createOcean();

    void initLayer( osg::Node* layer, osg::Vec3 color );

    void readLayers();
    void readLayer( const fdm::XmlNode &node, osg::Group *parent );
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_LAYERS_H
