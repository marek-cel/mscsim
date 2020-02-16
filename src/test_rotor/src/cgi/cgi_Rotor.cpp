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

#include <cgi/cgi_Rotor.h>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LineWidth>

#include <cgi/cgi_Models.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Rotor::Rotor() :
    _bladesCount ( 4 ),
    //_bladesCount ( 1 ),
    _bladesOffset ( 0.2 ),

    _direction ( Data::Rotor::CW )
{
    _root = new osg::Group();

    _mainRotor = new osg::PositionAttitudeTransform();
    _root->addChild( _mainRotor.get() );

    reload();
}

////////////////////////////////////////////////////////////////////////////////

Rotor::~Rotor() {}

////////////////////////////////////////////////////////////////////////////////

void Rotor::update()
{
    if ( _direction != Data::get()->rotor.direction )
    {
        reload();
    }

    double coef = 1.0;

    if ( _direction ==  Data::Rotor::CCW )
    {
        coef = -1.0;
    }

    for ( Datums::iterator it = _datums.begin(); it != _datums.end(); it++ )
    {
        if ( Data::get()->other.visible_blades_datum )
            (*it)->setAllChildrenOn();
        else
            (*it)->setAllChildrenOff();
    }

    // main rotor
    if ( _mainRotor.valid() )
    {
        double psi = coef * Data::get()->rotor.azimuth;
        _mainRotor->setAttitude( osg::Quat( psi, osg::Z_AXIS ) );
    }

    // main rotor blades
    for ( unsigned int i = 0; i < _blades.size() && i < MAX_BLADES; i++ )
    {
        double pitching = Data::get()->blade[ i ].theta * ( -coef );
        double flapping = Data::get()->blade[ i ].beta;

        _blades[ i ]->setAttitude( osg::Quat( pitching, osg::X_AXIS,
                                              flapping, osg::Y_AXIS,
                                                   0.0, osg::Z_AXIS ) );
    }

    _direction = Data::get()->rotor.direction;
}

////////////////////////////////////////////////////////////////////////////////

void Rotor::createBlades()
{
    osg::ref_ptr<osg::Group> blades = new osg::Group();
    blades->setName( "Blades" );
    _mainRotor->addChild( blades.get() );

    osg::ref_ptr<osg::Node> bladeNode;

    if ( Data::get()->rotor.direction == Data::Rotor::CCW )
        bladeNode = Models::get( "../data/uh60_blade.osgt" );
    else
        bladeNode = Models::get( "../data/w3wa_blade.osgt" );

    if ( bladeNode.valid() )
    {
        const double dirCoef = Data::get()->rotor.direction == Data::Rotor::CCW ? -1.0 : 1.0;
        const double angleStep = dirCoef*2.0*M_PI/((double)_bladesCount);

        for ( unsigned int i = 0; i < _bladesCount; i++ )
        {
            double azimuth = i*angleStep + M_PI;

            while ( azimuth < 0.0        ) azimuth += 2.0 * M_PI;
            while ( azimuth > 2.0 * M_PI ) azimuth -= 2.0 * M_PI;

            double offset_x = cos( azimuth ) * _bladesOffset;
            double offset_y = sin( azimuth ) * _bladesOffset;

            osg::ref_ptr<osg::PositionAttitudeTransform> patHinge = new osg::PositionAttitudeTransform();
            patHinge->setName( "BladeHinge" );
            patHinge->addChild( bladeNode.get() );

            osg::ref_ptr<osg::PositionAttitudeTransform> patBlade = new osg::PositionAttitudeTransform();
            patBlade->setName( "BladePAT" );
            patBlade->setAttitude( osg::Quat( azimuth, osg::Vec3( 0,0,1 ) ) );
            patBlade->setPosition( osg::Vec3( offset_x, offset_y, 0.0 ) );
            patBlade->addChild( patHinge.get() );

            blades->addChild( patBlade.get() );

            _blades.push_back( patHinge.get() );

            // datum line
            {
                osg::ref_ptr<osg::Switch> sw = new osg::Switch();
                patBlade->addChild( sw.get() );

                _datums.push_back( sw.get() );

                osg::ref_ptr<osg::Geode> geode = new osg::Geode();
                sw->addChild( geode.get() );

                osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

                osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // normals
                osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
                osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

                v->push_back( osg::Vec3( 0.0f, 0.0f, 0.0f ) );
                v->push_back( osg::Vec3( -dirCoef * 8.0, 0.0, 0.0 ) );
                n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );
                c->push_back( osg::Vec4( 0.0f, 1.0f, 0.0f, 1.0f ) );

                geometry->setVertexArray( v.get() );
                geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, 0, v->size() ) );
                geometry->setNormalArray( n.get() );
                geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );
                geometry->setColorArray( c.get() );
                geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

                geode->addDrawable( geometry.get() );

                osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth();
                lineWidth->setWidth( 1.0f );

                geode->getOrCreateStateSet()->setAttributeAndModes( lineWidth, osg::StateAttribute::ON );
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Rotor::reload()
{
    removeAllChildren();

    _root->addChild( _mainRotor.get() );

    osg::ref_ptr<osg::Node> nodeHub;

    if ( Data::get()->rotor.direction == Data::Rotor::CCW )
        nodeHub = Models::get( "../data/uh60_hub.osgt" );
    else
        nodeHub = Models::get( "../data/w3wa_hub.osgt" );

    if ( nodeHub.valid() ) _mainRotor->addChild( nodeHub.get() );

    createBlades();
}

////////////////////////////////////////////////////////////////////////////////

void Rotor::removeAllChildren()
{
    if ( _mainRotor.valid() )
    {
        _mainRotor->removeChild( 0, _mainRotor->getNumChildren() );
    }

    if ( _root.valid() )
    {
        _root->removeChild( 0, _root->getNumChildren() );
    }

    _blades.clear();
    _datums.clear();
}
