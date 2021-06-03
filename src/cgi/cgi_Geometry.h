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
#ifndef CGI_GEOMETRY_H
#define CGI_GEOMETRY_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief CGI geometry class.
 */
class Geometry
{
public:

    /** Dome projection types. */
    enum Projection
    {
        Azimuthal = 0,  ///< azimuthal projection
        Cylindrical     ///< cylindrical projection
    };

    /**
     * @brief Creates dome.
     * @param geom
     * @param radius
     * @param texCoords
     * @param projection
     * @param lat_segments
     * @param lon_segments
     */
    static void createDome( osg::Geometry *geom, double radius,
                            bool texCoords = false, Projection projection = Cylindrical,
                            int lat_segments = 18, int lon_segments = 36 );

    /**
     * @brief Creates round face.
     * @param geom
     * @param radius
     * @param texCoords
     * @param segments
     */
    static void createFace( osg::Geometry *geom, double radius, bool texCoords = false,
                            int segments = 32 );

    /**
     * @brief Creates line.
     * @param geom
     * @param b
     * @param e
     * @param color
     */
    static void createLine( osg::Geometry *geom, const osg::Vec3 &b, const osg::Vec3 &e,
                            const osg::Vec3 &color );

    /**
     * @brief Creates flat square plane.
     * @param geom
     * @param size
     * @param segments
     * @param origin_x
     * @param origin_y
     * @param texCoords
     * @param segmentsPerTexture
     */
    static void createPlane( osg::Geometry *geom, double size, int segments,
                             double origin_x = 0.0, double origin_y = 0.0,
                             bool texCoords = false, int segmentsPerTexture = 1 );

    /**
     * @brief Creates quad.
     * @param geom
     * @param v
     * @param texCoords
     * @param alpha
     */
    static void createQuad( osg::Geometry *geom, osg::Vec3Array *v,
                            bool texCoords = false, bool color = false,
                            double alpha = 1.0 );

    /**
     * @brief Creates ring.
     * @param geom
     * @param radius_inner
     * @param radius_outer
     * @param texCoords
     * @param segments
     */
    static void createRing( osg::Geometry *geom, double radius_i, double radius_o,
                            bool texCoords = false, int segments = 32 );
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_GEOMETRY_H
