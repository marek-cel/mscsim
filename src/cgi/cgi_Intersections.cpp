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

#include <cgi/cgi_Intersections.h>

#include <limits>

#include <osgDB/ReadFile>

#include <osgUtil/LineSegmentIntersector>

#include <cgi/cgi_WGS84.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

osg::Node* Intersections::ReadCallback::readNodeFile( const std::string &filename )
{
    return osgDB::readRefNodeFile( filename ).release();
}

////////////////////////////////////////////////////////////////////////////////

Intersections::Intersections()
{
    m_rc = new ReadCallback();

//    m_iv = new osgUtil::IntersectionVisitor();
//    m_iv->setLODSelectionMode( osgUtil::IntersectionVisitor::USE_HIGHEST_LEVEL_OF_DETAIL );
//    m_iv->setTraversalMode( osgUtil::IntersectionVisitor::TRAVERSE_ACTIVE_CHILDREN );
//    m_iv->setReadCallback( m_rc.get() );

//    m_ig = new osgUtil::IntersectorGroup();
//    m_iv->setIntersector( m_ig.get() );
}

////////////////////////////////////////////////////////////////////////////////

Intersections::~Intersections() {}

////////////////////////////////////////////////////////////////////////////////

bool Intersections::findFirst( const osg::Vec3d &b, const osg::Vec3d &e,
                               osg::Vec3d &r, osg::Vec3d &n )
{
//    m_iv->reset();
//    m_ig->clear();

    if ( m_scenery.valid() )
    {
        if ( ( e - b ).length2() > 0.0 )
        {
            osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector =
                    new osgUtil::LineSegmentIntersector( b, e );

            osgUtil::IntersectionVisitor iv( intersector.get(), m_rc.get() );
            iv.setLODSelectionMode( osgUtil::IntersectionVisitor::USE_HIGHEST_LEVEL_OF_DETAIL );
            iv.setTraversalMode( osgUtil::IntersectionVisitor::TRAVERSE_ACTIVE_CHILDREN );
            m_scenery->accept( iv );

//            osg::ref_ptr<osgUtil::IntersectionVisitor> iv = new osgUtil::IntersectionVisitor( intersector.get(), m_rc.get() );
//            iv->setLODSelectionMode( osgUtil::IntersectionVisitor::USE_HIGHEST_LEVEL_OF_DETAIL );
//            iv->setTraversalMode( osgUtil::IntersectionVisitor::TRAVERSE_ACTIVE_CHILDREN );
//            m_scenery->accept( *iv );

//            m_iv->setIntersector( intersector.get() );
//            m_ig->addIntersector( intersector.get() );

//            m_scenery->accept( *m_iv );

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
