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

#include <cgi/map/cgi_Traces.h>

#include <osg/Geode>
#include <osg/Geometry>

#include <cgi/cgi_Mercator.h>

#include <cgi/map/cgi_Map.h>

#include <Data.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Traces::Traces( const Module *parent ) :
    Module( parent ),
    _visible ( true ),
    _counter ( 0 ),
    _prevState ( fdm::DataOut::Idle )
{
    _switch = new osg::Switch();
    _root->addChild( _switch.get() );

    _positions = new osg::Vec3Array();
}

////////////////////////////////////////////////////////////////////////////////

Traces::~Traces() {}

////////////////////////////////////////////////////////////////////////////////

void Traces::update()
{
    if ( fdm::DataOut::Working == Data::get()->stateOut || fdm::DataOut::Working == _prevState )
    {
        if ( ( _visible && _counter % 10 == 0 ) || fdm::DataOut::Working != Data::get()->stateOut )
        {
            _counter = 0;

            if ( _switch->getNumChildren() > 0 )
            {
                _switch->removeChildren( 0, _switch->getNumChildren() );
            }

            _positions->push_back( osg::Vec3( Mercator::x( Data::get()->ownship.longitude ),
                                              Mercator::y( Data::get()->ownship.latitude ),
                                              Map::_zTraces ) );

            osg::ref_ptr<osg::Geode> geode = new osg::Geode();
            _switch->addChild( geode.get() );

            osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

            osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
            osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

            n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );
            c->push_back( osg::Vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );

            geometry->setVertexArray( _positions.get() );
            geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, 0, _positions->size() ) );
            geometry->setNormalArray( n.get() );
            geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );
            geometry->setColorArray( c.get() );
            geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

            geode->addDrawable( geometry.get() );
        }

        _counter++;
    }
    else if ( Data::get()->stateOut == fdm::DataOut::Idle )
    {
        reset();
    }

    _prevState = Data::get()->stateOut;
}

////////////////////////////////////////////////////////////////////////////////

void Traces::reset()
{
    _positions->clear();

    _counter = 0;

    if ( _switch->getNumChildren() > 0 )
    {
        _switch->removeChildren( 0, _switch->getNumChildren() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Traces::setVisibility( bool visible )
{
    _visible = visible;

    if ( visible )
    {
        _counter = 0;
        _switch->setAllChildrenOn();
    }
    else
    {
        _switch->setAllChildrenOff();

        if ( _switch->getNumChildren() > 0 )
        {
            _switch->removeChildren( 0, _switch->getNumChildren() );
        }
    }
}
