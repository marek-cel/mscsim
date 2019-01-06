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

#include <cgi/cgi_WGS84.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Intersections::Intersections() {}

////////////////////////////////////////////////////////////////////////////////

Intersections::~Intersections() {}

////////////////////////////////////////////////////////////////////////////////

bool Intersections::findFirst( const osg::Vec3d &b, const osg::Vec3d &e,
                               osg::Vec3d &r, osg::Vec3d &n )
{
    if ( m_scenery.valid() )
    {
        if ( ( e - b ).length2() > 0.0 )
        {
            osg::ref_ptr<osg::LineSegment> lineSegment = new osg::LineSegment( b, e );

            osgUtil::IntersectVisitor intersectVisitor;
            intersectVisitor.addLineSegment( lineSegment );
            m_scenery->accept( intersectVisitor );

            osgUtil::IntersectVisitor::HitList hitList = intersectVisitor.getHitList( lineSegment );

            if ( !hitList.empty() )
            {
                osgUtil::Hit hit = hitList.front();

                r = hit.getWorldIntersectPoint();
                n = hit.getWorldIntersectNormal();

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

    osg::ref_ptr<osg::LineSegment> lineSegment = new osg::LineSegment( b_wgs, e_wgs );

    osgUtil::IntersectVisitor intersectVisitor;
    intersectVisitor.addLineSegment( lineSegment );
    m_scenery->accept( intersectVisitor );

    osgUtil::IntersectVisitor::HitList hitList = intersectVisitor.getHitList( lineSegment );

    if ( !hitList.empty() )
    {
        osgUtil::Hit hit = hitList.front();

        return WGS84( hit.getWorldIntersectPoint() ).getAlt();
    }

    return std::numeric_limits< double >::quiet_NaN();
}
