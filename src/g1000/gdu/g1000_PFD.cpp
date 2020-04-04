/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
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

#include <g1000/gdu/g1000_PFD.h>

#include <g1000/g1000_Defines.h>
#include <g1000/g1000_Log.h>

#include <g1000/xml/g1000_XmlDoc.h>

////////////////////////////////////////////////////////////////////////////////

using namespace g1000;

////////////////////////////////////////////////////////////////////////////////

const osg::Vec3 PFD::_colorBars = osg::Vec3( 0.13, 0.13, 0.13 );
const double PFD::_z_bars = -50.0;

////////////////////////////////////////////////////////////////////////////////

PFD::PFD() :
    _adi ( nullptr ),
    _alt ( nullptr ),
    _asi ( nullptr ),
    _hsi ( nullptr ),
    _vsi ( nullptr )
{
    _adi = new ADI();
    _alt = new ALT();
    _asi = new ASI();
    _hsi = new HSI();
    _vsi = new VSI();

    _root->addChild( _adi->getNode() );
    _root->addChild( _alt->getNode() );
    _root->addChild( _asi->getNode() );
    _root->addChild( _hsi->getNode() );
    _root->addChild( _vsi->getNode() );
}

////////////////////////////////////////////////////////////////////////////////

PFD::~PFD()
{
    if ( _adi ) delete _adi;
    _adi = nullptr;

    if ( _alt ) delete _alt;
    _alt = nullptr;

    if ( _asi ) delete _asi;
    _asi = nullptr;

    if ( _hsi ) delete _hsi;
    _hsi = nullptr;

    if ( _vsi ) delete _vsi;
    _vsi = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

void PFD::init( const std::string &file )
{
    XmlDoc doc( file );

    if ( doc.isOpen() )
    {
        XmlNode rootNode = doc.getRootNode();

        if ( rootNode.isValid() )
        {
            XmlNode nodeADI = rootNode.getFirstChildElement( "adi" );
            XmlNode nodeALT = rootNode.getFirstChildElement( "alt" );
            XmlNode nodeASI = rootNode.getFirstChildElement( "asi" );
            XmlNode nodeHSI = rootNode.getFirstChildElement( "hsi" );
            XmlNode nodeVSI = rootNode.getFirstChildElement( "vsi" );

            XmlNode nodeBarLower = rootNode.getFirstChildElement( "bar_lower" );
            XmlNode nodeBarUpper = rootNode.getFirstChildElement( "bar_upper" );

            _adi->init( nodeADI );
            _alt->init( nodeALT );
            _asi->init( nodeASI );
            _hsi->init( nodeHSI );
            _vsi->init( nodeVSI );

            createLowerBar( nodeBarLower );
            createUpperBar( nodeBarUpper );
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

void PFD::update( const Data &data )
{
    _adi->update( data );
    _alt->update( data );
    _asi->update( data );
    _hsi->update( data );
    _vsi->update( data );
}

////////////////////////////////////////////////////////////////////////////////

void PFD::createLowerBar( XmlNode &node )
{
    const double h = 5.0;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _root->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    v->push_back( osg::Vec3( -G1000_GDU_WIDTH_2, 0.0 - G1000_GDU_HEIGHT_2, _z_bars ) );
    v->push_back( osg::Vec3(  G1000_GDU_WIDTH_2, 0.0 - G1000_GDU_HEIGHT_2, _z_bars ) );
    v->push_back( osg::Vec3(  G1000_GDU_WIDTH_2,   h - G1000_GDU_HEIGHT_2, _z_bars ) );
    v->push_back( osg::Vec3( -G1000_GDU_WIDTH_2,   h - G1000_GDU_HEIGHT_2, _z_bars ) );

    c->push_back( osg::Vec4( _colorBars, 1.0 ) );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, v->size() ) );

    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );
}

////////////////////////////////////////////////////////////////////////////////

void PFD::createUpperBar( XmlNode &node )
{
    const double h = 11.0;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _root->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    v->push_back( osg::Vec3( -G1000_GDU_WIDTH_2, G1000_GDU_HEIGHT_2 - h , _z_bars ) );
    v->push_back( osg::Vec3(  G1000_GDU_WIDTH_2, G1000_GDU_HEIGHT_2 - h , _z_bars ) );
    v->push_back( osg::Vec3(  G1000_GDU_WIDTH_2, G1000_GDU_HEIGHT_2     , _z_bars ) );
    v->push_back( osg::Vec3( -G1000_GDU_WIDTH_2, G1000_GDU_HEIGHT_2     , _z_bars ) );

    c->push_back( osg::Vec4( _colorBars, 1.0 ) );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, v->size() ) );

    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );
}
