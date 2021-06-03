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

#include <cgi/cgi_Geometry.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

void Geometry::createDome( osg::Geometry *geom, double radius,
                           bool texCoords, Projection projection,
                           int lat_segments, int lon_segments )
{
    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec2Array> t = new osg::Vec2Array();
    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    double lat_step = osg::DegreesToRadians( 180.0 ) / (double)lat_segments;
    double lon_step = osg::DegreesToRadians( 360.0 ) / (double)lon_segments;

    double lat_prev = osg::DegreesToRadians( 90.0 );
    double lat_next = osg::DegreesToRadians( 90.0 ) - lat_step;
    double lon_prev = 0.0;

    double x_prev = 0.0;
    double y_prev = 0.0;
    double z_prev = radius;

    double x_next = 0.0;
    double y_next = 0.0;
    double z_next = radius;

    double r_prev = 0.0;
    double r_next = 0.0;

    for ( int i_lat = 0; i_lat < lat_segments; i_lat++ )
    {
        if ( lat_next < osg::DegreesToRadians( -90.0 ) )
        {
            lat_next = osg::DegreesToRadians( -90.0 );
        }

        z_prev = radius * sin( lat_prev );
        r_prev = radius * cos( lat_prev );

        for ( int i_lon = 0; i_lon < lon_segments + 1; i_lon++ )
        {
            lon_prev = i_lon * lon_step;

            x_prev = r_prev * cos( lon_prev - M_PI );
            y_prev = r_prev * sin( lon_prev - M_PI );

            z_next = radius * sin( lat_next );
            r_next = radius * cos( lat_next );

            x_next = r_next * cos( lon_prev - M_PI );
            y_next = r_next * sin( lon_prev - M_PI );

            osg::Vec3d p_prev( x_prev, y_prev, z_prev );
            osg::Vec3d p_next( x_next, y_next, z_next );

            v->push_back( p_prev );
            v->push_back( p_next );

            if ( texCoords )
            {
                if ( projection == Azimuthal )
                {
                    double n_arc_prev = ( M_PI_2 - fabs( lat_prev ) ) / M_PI_2;
                    double n_arc_next = ( M_PI_2 - fabs( lat_next ) ) / M_PI_2;

                    double tx_prev = 0.5 + 0.5 * n_arc_prev * sin( lon_prev );
                    double ty_prev = 0.5 + 0.5 * n_arc_prev * cos( lon_prev );
                    double tx_next = 0.5 + 0.5 * n_arc_next * sin( lon_prev );
                    double ty_next = 0.5 + 0.5 * n_arc_next * cos( lon_prev );

                    t->push_back( osg::Vec2( tx_prev, ty_prev ) );
                    t->push_back( osg::Vec2( tx_next, ty_next ) );
                }
                else if ( projection == Cylindrical )
                {
                    t->push_back( osg::Vec2( (double)i_lon/lon_segments, (double)(i_lat  )/lat_segments ) );
                    t->push_back( osg::Vec2( (double)i_lon/lon_segments, (double)(i_lat+1)/lat_segments ) );
                }
            }

            p_prev.normalize();
            p_next.normalize();

            n->push_back( -p_prev );
            n->push_back( -p_next );
        }

        lat_prev -= lat_step;
        lat_next -= lat_step;
    }

    c->push_back( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );

    geom->setVertexArray( v.get() );
    geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLE_STRIP, 0, v->size() ) );

    geom->setNormalArray( n.get() );
    geom->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );

    geom->setColorArray( c.get() );
    geom->setColorBinding( osg::Geometry::BIND_OVERALL );

    if ( texCoords ) geom->setTexCoordArray( 0, t.get() );
}

////////////////////////////////////////////////////////////////////////////////

void Geometry::createFace( osg::Geometry *geom, double radius, bool texCoords,
                           int segments )
{
    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec2Array> t = new osg::Vec2Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    v->push_back( osg::Vec3( 0.0, 0.0, 0.0 ) );
    if ( texCoords ) t->push_back( osg::Vec2( 0.5, 0.5 ) );

    double step = 2.0 * M_PI / (double)segments;

    for ( int i = 0; i < segments + 1; i++ )
    {
        double a = i * step;

        double sinA = sin( a );
        double cosA = cos( a );

        double y = radius * sinA;
        double z = radius * cosA;

        v->push_back( osg::Vec3( 0.0, y, z ) );
        if ( texCoords ) t->push_back( osg::Vec2( 1.0 - ( sinA + 1.0 ) / 2.0, ( cosA + 1.0 ) / 2.0 ) );
    }

    n->push_back( osg::Vec3( -1.0, 0.0, 0.0 ) );

    c->push_back( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );

    geom->setVertexArray( v.get() );
    geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLE_FAN, 0, v->size() ) );

    geom->setNormalArray( n.get() );
    geom->setNormalBinding( osg::Geometry::BIND_OVERALL );

    geom->setColorArray( c.get() );
    geom->setColorBinding( osg::Geometry::BIND_OVERALL );

    if ( texCoords ) geom->setTexCoordArray( 0, t.get() );
}

////////////////////////////////////////////////////////////////////////////////

void Geometry::createLine( osg::Geometry *geom, const osg::Vec3 &b, const osg::Vec3 &e,
                           const osg::Vec3 &color )
{
    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // normals
    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

    v->push_back( b );
    v->push_back( e );

    n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );
    c->push_back( osg::Vec4( color, 1.0f ) );

    geom->setVertexArray( v.get() );
    geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, 0, v->size() ) );
    geom->setNormalArray( n.get() );
    geom->setNormalBinding( osg::Geometry::BIND_OVERALL );
    geom->setColorArray( c.get() );
    geom->setColorBinding( osg::Geometry::BIND_OVERALL );
}

////////////////////////////////////////////////////////////////////////////////

void Geometry::createPlane( osg::Geometry *geom, double size, int segments,
                            double origin_x, double origin_y,
                            bool texCoords, int segmentsPerTexture )
{
    double step = (int)( size / segments );
    double texPerSegment = 1.0 / (double)segmentsPerTexture;

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec2Array> t = new osg::Vec2Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    for ( int ix = 0; ix < segments; ix++ )
    {
        for ( int iy = 0; iy < segments; iy++ )
        {
            bool even = false;

            if ( ( ix % 2 == 0 && iy % 2 != 0 )
              || ( ix % 2 != 0 && iy % 2 == 0 ) )
            {
                even = true;
            }

            double x0 = ( ix + 0 ) * step;
            double x1 = ( ix + 1 ) * step;
            double y0 = ( iy + 0 ) * step;
            double y1 = ( iy + 1 ) * step;

            double tx0 = ( ix + 0 ) % segmentsPerTexture * texPerSegment;
            double tx1 = ( ix + 1 ) % segmentsPerTexture * texPerSegment;
            double ty0 = ( iy + 0 ) % segmentsPerTexture * texPerSegment;
            double ty1 = ( iy + 1 ) % segmentsPerTexture * texPerSegment;

            if ( tx1 < 0.001 ) tx1 = 1.0;
            if ( ty1 < 0.001 ) ty1 = 1.0;

            osg::Vec3d r1( x0 + origin_x, y0 + origin_y, 0.0 );
            osg::Vec3d r2( x1 + origin_x, y0 + origin_y, 0.0 );
            osg::Vec3d r3( x0 + origin_x, y1 + origin_y, 0.0 );
            osg::Vec3d r4( x1 + origin_x, y1 + origin_y, 0.0 );

            osg::Vec2 t1( tx0, ty0 );
            osg::Vec2 t2( tx1, ty0 );
            osg::Vec2 t3( tx0, ty1 );
            osg::Vec2 t4( tx1, ty1 );

            if ( even )
            {
                v->push_back( r1 );
                v->push_back( r2 );
                v->push_back( r3 );

                v->push_back( r3 );
                v->push_back( r2 );
                v->push_back( r4 );

                if ( texCoords )
                {
                    t->push_back( t1 );
                    t->push_back( t2 );
                    t->push_back( t3 );

                    t->push_back( t3 );
                    t->push_back( t2 );
                    t->push_back( t4 );
                }
            }
            else
            {
                v->push_back( r1 );
                v->push_back( r2 );
                v->push_back( r4 );

                v->push_back( r1 );
                v->push_back( r4 );
                v->push_back( r3 );

                if ( texCoords )
                {
                    t->push_back( t1 );
                    t->push_back( t2 );
                    t->push_back( t4 );

                    t->push_back( t1 );
                    t->push_back( t4 );
                    t->push_back( t3 );
                }
            }
        }
    }

    n->push_back( osg::Vec3( -1.0f, 0.0f, 0.0f ) );

    c->push_back( osg::Vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );

    geom->setVertexArray( v.get() );
    geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLES, 0, v->size() ) );

    geom->setNormalArray( n.get() );
    geom->setNormalBinding( osg::Geometry::BIND_OVERALL );

    geom->setColorArray( c.get() );
    geom->setColorBinding( osg::Geometry::BIND_OVERALL );

    if ( texCoords ) geom->setTexCoordArray( 0, t.get() );
}

////////////////////////////////////////////////////////////////////////////////

void Geometry::createQuad( osg::Geometry *geom, osg::Vec3Array *v,
                           bool texCoords, bool color, double alpha )
{
    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec2Array> t = new osg::Vec2Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    osg::Vec3d tmp = ( v->at( 1 ) - v->at( 0 ) ) ^ ( v->at( 2 ) - v->at( 0 ) );
    tmp.normalize();
    n->push_back( tmp );

    c->push_back( osg::Vec4( 1.0f, 1.0f, 1.0f, alpha ) );

    geom->setVertexArray( v );
    geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLE_FAN, 0, v->size() ) );

    geom->setNormalArray( n.get() );
    geom->setNormalBinding( osg::Geometry::BIND_OVERALL );

    if ( color )
    {
        geom->setColorArray( c.get() );
        geom->setColorBinding( osg::Geometry::BIND_OVERALL );
    }

    if ( texCoords )
    {
        t->push_back( osg::Vec2( 0, 0 ) );
        t->push_back( osg::Vec2( 1, 0 ) );
        t->push_back( osg::Vec2( 1, 1 ) );
        t->push_back( osg::Vec2( 0, 1 ) );

        geom->setTexCoordArray( 0, t.get() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Geometry::createRing( osg::Geometry *geom, double radius_i, double radius_o,
                           bool texCoords, int segments )
{
    const double coef = radius_o / radius_i;

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec2Array> t = new osg::Vec2Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    double step = 2.0 * M_PI / (double)segments;

    for ( int i = 0; i < segments + 1; i++ )
    {
        double a = i * step;

        double sinA = sin( a );
        double cosA = cos( a );

        double x_i = radius_i * sinA;
        double y_i = radius_i * cosA;

        double x_o = radius_o * sinA;
        double y_o = radius_o * cosA;

        v->push_back( osg::Vec3( x_i, y_i, 0.0 ) );
        v->push_back( osg::Vec3( x_o, y_o, 0.0 ) );

        if ( texCoords )
        {
            double x = 1.0f - ( sinA + 1.0 ) / 2.0;
            double y = ( cosA + 1.0 ) / 2.0;

            t->push_back( osg::Vec2( x        , y        ) );
            t->push_back( osg::Vec2( x * coef , y * coef ) );
        }
    }

    n->push_back( osg::Vec3( 0.0, 0.0, 1.0 ) );

    c->push_back( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );

    geom->setVertexArray( v.get() );
    geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUAD_STRIP, 0, v->size() ) );

    geom->setNormalArray( n.get() );
    geom->setNormalBinding( osg::Geometry::BIND_OVERALL );

    geom->setColorArray( c.get() );
    geom->setColorBinding( osg::Geometry::BIND_OVERALL );

    if ( texCoords ) geom->setTexCoordArray( 0, t.get() );
}
