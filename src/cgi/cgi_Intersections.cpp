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

#include <cgi/cgi_Intersections.h>

#include <limits>

#include <osgDB/ReadFile>

#include <osgUtil/LineSegmentIntersector>

#include <cgi/cgi_WGS84.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

#if OPENSCENEGRAPH_SOVERSION < 140
osg::Node* Intersections::ReadCallback::readNodeFile( const std::string& filename )
{
    return osgDB::readRefNodeFile( filename ).release();
}
#else
osg::ref_ptr<osg::Node> Intersections::ReadCallback::readNodeFile( const std::string& filename )
{
    return osgDB::readRefNodeFile( filename );
}
#endif

////////////////////////////////////////////////////////////////////////////////

Intersections::Intersections()
{
    _rc = new ReadCallback();

//    _iv = new osgUtil::IntersectionVisitor();
//    _iv->setLODSelectionMode( osgUtil::IntersectionVisitor::USE_HIGHEST_LEVEL_OF_DETAIL );
//    _iv->setTraversalMode( osgUtil::IntersectionVisitor::TRAVERSE_ACTIVE_CHILDREN );
//    _iv->setReadCallback( _rc.get() );

//    _ig = new osgUtil::IntersectorGroup();
//    _iv->setIntersector( _ig.get() );
}

////////////////////////////////////////////////////////////////////////////////

Intersections::~Intersections() {}

////////////////////////////////////////////////////////////////////////////////

bool Intersections::findFirst( const osg::Vec3d &b, const osg::Vec3d &e,
                               osg::Vec3d &r, osg::Vec3d &n )
{
//    _iv->reset();
//    _ig->clear();

    if ( _scenery.valid() )
    {
        if ( ( e - b ).length2() > 1.0e-6 )
        {
            //osgUtil::LineSegmentIntersector tmp( b, e );
            //osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector = &tmp;

            osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector =
                    new osgUtil::LineSegmentIntersector( b, e );

            osgUtil::IntersectionVisitor iv( intersector.get(), _rc.get() );
            iv.setLODSelectionMode( osgUtil::IntersectionVisitor::USE_HIGHEST_LEVEL_OF_DETAIL );
            iv.setTraversalMode( osgUtil::IntersectionVisitor::TRAVERSE_ACTIVE_CHILDREN );
            _scenery->accept( iv );

//            osg::ref_ptr<osgUtil::IntersectionVisitor> iv = new osgUtil::IntersectionVisitor( intersector.get(), _rc.get() );
//            iv->setLODSelectionMode( osgUtil::IntersectionVisitor::USE_HIGHEST_LEVEL_OF_DETAIL );
//            iv->setTraversalMode( osgUtil::IntersectionVisitor::TRAVERSE_ACTIVE_CHILDREN );
//            _scenery->accept( *iv );

//            _iv->setIntersector( intersector.get() );
//            _ig->addIntersector( intersector.get() );

//            _scenery->accept( *_iv );

            if ( intersector->containsIntersections() )
            {
                osgUtil::LineSegmentIntersector::Intersection intersection = intersector->getFirstIntersection();

                r = intersection.getWorldIntersectPoint();
                n = intersection.getWorldIntersectNormal();

                return true;
            }
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

double Intersections::getElevation( double lat, double lon )
{
    osg::Vec3d b_wgs = WGS84( lat, lon, 10000.0 ).getPosition();
    osg::Vec3d e_wgs = WGS84( lat, lon, -1000.0 ).getPosition();
    osg::Vec3d r_wgs;
    osg::Vec3d n_wgs;

    if ( findFirst( b_wgs, e_wgs, r_wgs, n_wgs ) )
    {
        return WGS84( r_wgs ).getAlt();
    }

    return std::numeric_limits< double >::quiet_NaN();
}
