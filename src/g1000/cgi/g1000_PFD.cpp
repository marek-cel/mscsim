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

#include <g1000/cgi/g1000_PFD.h>

#include <g1000/g1000_Defines.h>
#include <g1000/g1000_Log.h>

#include <g1000/cgi/g1000_Colors.h>
#include <g1000/cgi/g1000_Fonts.h>

#include <g1000/xml/g1000_XmlDoc.h>

////////////////////////////////////////////////////////////////////////////////

using namespace g1000;

////////////////////////////////////////////////////////////////////////////////

const double PFD::_z_bars = -50.0;

////////////////////////////////////////////////////////////////////////////////

PFD::PFD( IFD *ifd, const char *file ) :
    GDU ( ifd ),

    _adi  ( NULLPTR ),
    _afcs ( NULLPTR ),
    _alt  ( NULLPTR ),
    _asi  ( NULLPTR ),
    _com  ( NULLPTR ),
    _hsi  ( NULLPTR ),
    _nav  ( NULLPTR ),
    _vsi  ( NULLPTR ),
    _wypt ( NULLPTR )
{
    XmlDoc doc( file );

    if ( doc.isOpen() )
    {
        XmlNode rootNode = doc.getRootNode();

        if ( rootNode.isValid() )
        {
            XmlNode nodeASI = rootNode.getFirstChildElement( "asi" );

            _adi  = new ADI  ( ifd );
            _afcs = new AFCS ( ifd );
            _alt  = new ALT  ( ifd );
            _asi  = new ASI  ( ifd, nodeASI );
            _com  = new COM  ( ifd );
            _hsi  = new HSI  ( ifd );
            _nav  = new NAV  ( ifd );
            _vsi  = new VSI  ( ifd );
            _wypt = new WYPT ( ifd );

            _root->addChild( _adi  ->getNode() );
            _root->addChild( _afcs ->getNode() );
            _root->addChild( _alt  ->getNode() );
            _root->addChild( _asi  ->getNode() );
            _root->addChild( _com  ->getNode() );
            _root->addChild( _hsi  ->getNode() );
            _root->addChild( _nav  ->getNode() );
            _root->addChild( _vsi  ->getNode() );
            _root->addChild( _wypt ->getNode() );

            createLowerBar();
            createUpperBar();
        }
        else
        {
            Log::e() << "Reading file \"" << file << "\" failed. Invalid root node." << std::endl;
        }
    }
    else
    {
        Log::e() << "Reading file \"" << file << "\" failed." << std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////

PFD::~PFD()
{
    DELPTR( _adi  );
    DELPTR( _afcs );
    DELPTR( _alt  );
    DELPTR( _asi  );
    DELPTR( _com  );
    DELPTR( _hsi  );
    DELPTR( _nav  );
    DELPTR( _vsi  );
    DELPTR( _wypt );
}

////////////////////////////////////////////////////////////////////////////////

void PFD::update()
{
    _adi  ->update();
    _afcs ->update();
    _alt  ->update();
    _asi  ->update();
    _hsi  ->update();
    _vsi  ->update();
    _wypt ->update();
}

////////////////////////////////////////////////////////////////////////////////

void PFD::createLowerBar()
{
    const double h = 5.0;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _root->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry_f = new osg::Geometry();
    osg::ref_ptr<osg::Geometry> geometry_b = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> vf = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> cf = new osg::Vec4Array();

    osg::ref_ptr<osg::Vec3Array> vb = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> cb = new osg::Vec4Array();

    vf->push_back( osg::Vec3( -G1000_GDU_WIDTH_2, 0.0 - G1000_GDU_HEIGHT_2, _z_bars ) );
    vf->push_back( osg::Vec3(  G1000_GDU_WIDTH_2, 0.0 - G1000_GDU_HEIGHT_2, _z_bars ) );
    vf->push_back( osg::Vec3(  G1000_GDU_WIDTH_2,   h - G1000_GDU_HEIGHT_2, _z_bars ) );
    vf->push_back( osg::Vec3( -G1000_GDU_WIDTH_2,   h - G1000_GDU_HEIGHT_2, _z_bars ) );

    vb->push_back( osg::Vec3(  G1000_GDU_WIDTH_2,   h - G1000_GDU_HEIGHT_2, _z_bars + 1.0 ) );
    vb->push_back( osg::Vec3( -G1000_GDU_WIDTH_2,   h - G1000_GDU_HEIGHT_2, _z_bars + 1.0 ) );

    const int steps = 12;
    const double step = G1000_GDU_WIDTH / (double)(steps);
    for ( int i = 0; i <= steps; i++ )
    {
        double x = floor( -G1000_GDU_WIDTH_2 + i * step + 0.5 );

        vb->push_back( osg::Vec3( x, 0.0 - G1000_GDU_HEIGHT_2, _z_bars + 1.0 ) );
        vb->push_back( osg::Vec3( x,   h - G1000_GDU_HEIGHT_2, _z_bars + 1.0 ) );
    }

    cf->push_back( osg::Vec4( Colors::_black, 1.0 ) );
    cb->push_back( osg::Vec4( Colors::_white, 1.0 ) );

    geometry_f->setVertexArray( vf.get() );
    geometry_f->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, vf->size() ) );

    geometry_f->setColorArray( cf.get() );
    geometry_f->setColorBinding( osg::Geometry::BIND_OVERALL );

    geometry_b->setVertexArray( vb.get() );
    geometry_b->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, vb->size() ) );

    geometry_b->setColorArray( cb.get() );
    geometry_b->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry_f.get() );
    geode->addDrawable( geometry_b.get() );
}

////////////////////////////////////////////////////////////////////////////////

void PFD::createUpperBar()
{
    const double h = 11.0;

    const double dx_freq  = 51.0;
    const double dx_lat   = 25.0;
    const double dx_ver   = 55.0;
    const double dx_wypt  = 55.0;

    osg::ref_ptr<osg::Geode> geode_f = new osg::Geode();
    osg::ref_ptr<osg::Geode> geode_b = new osg::Geode();

    _root->addChild( geode_f.get() );
    _root->addChild( geode_b.get() );

    osg::ref_ptr<osg::Geometry> geometry_f = new osg::Geometry();
    osg::ref_ptr<osg::Geometry> geometry_b = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> vf = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> cf = new osg::Vec4Array();

    osg::ref_ptr<osg::Vec3Array> vb = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> cb = new osg::Vec4Array();

    vf->push_back( osg::Vec3( -G1000_GDU_WIDTH_2, G1000_GDU_HEIGHT_2 - h , _z_bars ) );
    vf->push_back( osg::Vec3(  G1000_GDU_WIDTH_2, G1000_GDU_HEIGHT_2 - h , _z_bars ) );
    vf->push_back( osg::Vec3(  G1000_GDU_WIDTH_2, G1000_GDU_HEIGHT_2     , _z_bars ) );
    vf->push_back( osg::Vec3( -G1000_GDU_WIDTH_2, G1000_GDU_HEIGHT_2     , _z_bars ) );

    vb->push_back( osg::Vec3( -G1000_GDU_WIDTH_2, G1000_GDU_HEIGHT_2 - h , _z_bars + 1.0 ) );
    vb->push_back( osg::Vec3(  G1000_GDU_WIDTH_2, G1000_GDU_HEIGHT_2 - h , _z_bars + 1.0 ) );

    vb->push_back( osg::Vec3( -G1000_GDU_WIDTH_2 + dx_freq, G1000_GDU_HEIGHT_2     , _z_bars + 1.0 ) );
    vb->push_back( osg::Vec3( -G1000_GDU_WIDTH_2 + dx_freq, G1000_GDU_HEIGHT_2 - h , _z_bars + 1.0 ) );

    vb->push_back( osg::Vec3(  G1000_GDU_WIDTH_2 - dx_freq, G1000_GDU_HEIGHT_2     , _z_bars + 1.0 ) );
    vb->push_back( osg::Vec3(  G1000_GDU_WIDTH_2 - dx_freq, G1000_GDU_HEIGHT_2 - h , _z_bars + 1.0 ) );

    vb->push_back( osg::Vec3( -G1000_GDU_WIDTH_2 + dx_freq, G1000_GDU_HEIGHT_2 - 0.5 * h , _z_bars + 1.0 ) );
    vb->push_back( osg::Vec3(  G1000_GDU_WIDTH_2 - dx_freq, G1000_GDU_HEIGHT_2 - 0.5 * h , _z_bars + 1.0 ) );

    vb->push_back( osg::Vec3( -G1000_GDU_WIDTH_2 + dx_freq + dx_lat, G1000_GDU_HEIGHT_2 -       h , _z_bars + 1.0 ) );
    vb->push_back( osg::Vec3( -G1000_GDU_WIDTH_2 + dx_freq + dx_lat, G1000_GDU_HEIGHT_2 - 0.5 * h , _z_bars + 1.0 ) );

    vb->push_back( osg::Vec3(  G1000_GDU_WIDTH_2 - dx_freq - dx_ver, G1000_GDU_HEIGHT_2 -       h , _z_bars + 1.0 ) );
    vb->push_back( osg::Vec3(  G1000_GDU_WIDTH_2 - dx_freq - dx_ver, G1000_GDU_HEIGHT_2 - 0.5 * h , _z_bars + 1.0 ) );

    vb->push_back( osg::Vec3( -G1000_GDU_WIDTH_2 + dx_freq + dx_wypt, G1000_GDU_HEIGHT_2           , _z_bars + 1.0 ) );
    vb->push_back( osg::Vec3( -G1000_GDU_WIDTH_2 + dx_freq + dx_wypt, G1000_GDU_HEIGHT_2 - 0.5 * h , _z_bars + 1.0 ) );

    cf->push_back( osg::Vec4( Colors::_black, 1.0 ) );
    cb->push_back( osg::Vec4( Colors::_white, 1.0 ) );

    geometry_f->setVertexArray( vf.get() );
    geometry_f->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, vf->size() ) );

    geometry_f->setColorArray( cf.get() );
    geometry_f->setColorBinding( osg::Geometry::BIND_OVERALL );

    geometry_b->setVertexArray( vb.get() );
    geometry_b->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, vb->size() ) );

    geometry_b->setColorArray( cb.get() );
    geometry_b->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode_f->addDrawable( geometry_f.get() );
    geode_b->addDrawable( geometry_b.get() );
}
