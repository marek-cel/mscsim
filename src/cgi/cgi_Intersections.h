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
#ifndef CGI_INTERSECTIONS_H
#define CGI_INTERSECTIONS_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/Node>

#include <osgUtil/IntersectionVisitor>

#include <cgi/cgi_Singleton.h>

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
        virtual osg::Node* readNodeFile( const std::string &filename );
    };

    /** Destructor. */
    virtual ~Intersections();

    /**
     * Finds intersections.
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
        m_scenery = scenery;
    }

private:

    osg::ref_ptr<osg::Node> m_scenery;

    osg::ref_ptr<ReadCallback> m_rc;

//    osg::ref_ptr<osgUtil::IntersectionVisitor> m_iv;
//    osg::ref_ptr<osgUtil::IntersectorGroup>    m_ig;

    /**
     * You should use static function instance() due to get refernce
     * to Random class instance.
     */
    Intersections();

    /** Using this constructor is forbidden. */
    Intersections( const Intersections & ) {}
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_INTERSECTIONS_H
