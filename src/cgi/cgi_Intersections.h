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
#ifndef CGI_INTERSECTIONS_H
#define CGI_INTERSECTIONS_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/Node>
#include <osg/Vec3d>
#include <osg/Version>

#include <osgUtil/IntersectionVisitor>

#include <sim/Singleton.h>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief Intersections class.
 */
class Intersections : public Singleton< Intersections >
{
    friend class Singleton< Intersections >;

public:

    struct ReadCallback : public osgUtil::IntersectionVisitor::ReadCallback
    {
#       if OPENSCENEGRAPH_SOVERSION < 140
        virtual osg::Node* readNodeFile( const std::string& filename );
#       else
        virtual osg::ref_ptr<osg::Node> readNodeFile( const std::string& filename );
#       endif
    };

private:

    /**
     * You should use static function instance() due to get refernce
     * to Intersections class instance.
     */
    Intersections();

    /** Using this constructor is forbidden. */
    Intersections( const Intersections & ) {}

public:

    /** @brief Destructor. */
    virtual ~Intersections();

    /**
     * @brief Finds intersections.
     * @param b segement line beginning coordinates
     * @param e segement line ending coordinates
     * @param r intersection coordinates
     * @param n intersection normal
     * @return true on success, false on failure
     */
    bool findFirst( const osg::Vec3d &b, const osg::Vec3d &e,
                    osg::Vec3d &r, osg::Vec3d &n );

    /** */
    double getElevation( double lat, double lon );

    /** */
    inline void setScenery( osg::Node *scenery )
    {
        _scenery = scenery;
    }

private:

    osg::ref_ptr<osg::Node> _scenery;
    osg::ref_ptr<ReadCallback> _rc;

//    osg::ref_ptr<osgUtil::IntersectionVisitor> _iv;
//    osg::ref_ptr<osgUtil::IntersectorGroup>    _ig;
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_INTERSECTIONS_H
