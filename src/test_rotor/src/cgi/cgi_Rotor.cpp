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

#include <defs.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Rotor::Rotor() :
    _bladesOffset ( 0.2 ),

    _rotorRadiusCW  ( 7.885 ),
    _rotorRadiusCCW ( 8.14 ),

    _direction ( Data::Rotor::CW ),

    _bladesCount ( 0 )
{
    _root = new osg::Group();

    _mainRotor = new osg::PositionAttitudeTransform();
    _root->addChild( _mainRotor.get() );

    _switchPaths = new osg::Switch();
    _root->addChild( _switchPaths.get() );

    for ( int i = 0; i < MAX_BLADES; i++ )
    {
        _paths.push_back( new osg::Vec3Array() );
    }

    reload();
}

////////////////////////////////////////////////////////////////////////////////

Rotor::~Rotor() {}

////////////////////////////////////////////////////////////////////////////////

void Rotor::update()
{
    if ( _direction != Data::get()->rotor.direction
      || _bladesCount != Data::get()->blades_count )
    {
        _direction = Data::get()->rotor.direction;
        _bladesCount = Data::get()->blades_count;

        reload();
    }

    double coef = 1.0;

    if ( _direction ==  Data::Rotor::CCW )
    {
        coef = -1.0;
    }

    for ( Datums::iterator it = _datums.begin(); it != _datums.end(); it++ )
    {
        if ( Data::get()->other.show_blades_datum )
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

    updatePaths();
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
        const double angleStep = dirCoef*2.0*M_PI/((double)MAX_BLADES);

        double r = _direction == Data::Rotor::CW ? _rotorRadiusCW : _rotorRadiusCCW;
        double b = r - _bladesOffset;

        for ( int i = 0; i < Data::get()->blades_count; i++ )
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
                v->push_back( osg::Vec3( b, 0.0, 0.0 ) );
                n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );
                c->push_back( osg::Vec4( 0.0f, 1.0f, 1.0f, 1.0f ) );

                geometry->setVertexArray( v.get() );
                geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, 0, v->size() ) );
                geometry->setNormalArray( n.get() );
                geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );
                geometry->setColorArray( c.get() );
                geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

                geode->addDrawable( geometry.get() );

                osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth();
                lineWidth->setWidth( 0.8f );

                geode->getOrCreateStateSet()->setAttributeAndModes( lineWidth, osg::StateAttribute::ON );
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Rotor::createDatums()
{
    const double r = _direction == Data::Rotor::CW ? _rotorRadiusCW : _rotorRadiusCCW;

    // ring
    {
        osg::ref_ptr<osg::Switch> sw = new osg::Switch();
        _root->addChild( sw.get() );

        _datums.push_back( sw.get() );

        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        sw->addChild( geode.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // normals
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
        osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );
        c->push_back( osg::Vec4( 1.0f, 0.0f, 1.0f, 1.0f ) );

        for ( int i = 0; i < 36; i++ )
        {
            double a = osg::DegreesToRadians( 10.0 * i );
            double x = r * sin( a );
            double y = r * cos( a );

            v->push_back( osg::Vec3( x, y, 0.0 ) );
        }

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_LOOP, 0, v->size() ) );
        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );
        geometry->setColorArray( c.get() );
        geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );

        osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth();
        lineWidth->setWidth( 2.0f );

        geode->getOrCreateStateSet()->setAttributeAndModes( lineWidth, osg::StateAttribute::ON );
    }

    // lines
    {
        osg::ref_ptr<osg::Switch> sw = new osg::Switch();
        _root->addChild( sw.get() );

        _datums.push_back( sw.get() );

        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        sw->addChild( geode.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // normals
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
        osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );
        c->push_back( osg::Vec4( 1.0f, 0.0f, 1.0f, 1.0f ) );

        v->push_back( osg::Vec3( -r, 0.0, 0.0 ) );
        v->push_back( osg::Vec3(  r, 0.0, 0.0 ) );
        v->push_back( osg::Vec3( 0.0, -r, 0.0 ) );
        v->push_back( osg::Vec3( 0.0,  r, 0.0 ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );
        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );
        geometry->setColorArray( c.get() );
        geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );

        osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth();
        lineWidth->setWidth( 2.0f );

        geode->getOrCreateStateSet()->setAttributeAndModes( lineWidth, osg::StateAttribute::ON );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Rotor::reload()
{
    removeAllChildren();

    _root->addChild( _mainRotor.get() );
    _root->addChild( _switchPaths.get() );

    osg::ref_ptr<osg::Node> nodeHub;

    if ( Data::get()->rotor.direction == Data::Rotor::CCW )
        nodeHub = Models::get( "../data/uh60_hub.osgt" );
    else
        nodeHub = Models::get( "../data/w3wa_hub.osgt" );

    if ( nodeHub.valid() ) _mainRotor->addChild( nodeHub.get() );

    createBlades();
    createDatums();
}

////////////////////////////////////////////////////////////////////////////////

void Rotor::removeAllChildren()
{
    if ( _mainRotor.valid() )
    {
        _mainRotor->removeChild( 0, _mainRotor->getNumChildren() );
    }

    if ( _switchPaths.valid() )
    {
        _switchPaths->removeChild( 0, _switchPaths->getNumChildren() );
    }

    if ( _root.valid() )
    {
        _root->removeChild( 0, _root->getNumChildren() );
    }

    _blades.clear();
    _datums.clear();

    for ( Paths::iterator it = _paths.begin(); it != _paths.end(); it++ )
    {
        (*it)->clear();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Rotor::updatePaths()
{
    if ( _switchPaths->getNumChildren() > 0 )
    {
        _switchPaths->removeChildren( 0, _switchPaths->getNumChildren() );
    }

    if ( Data::get()->phase == Data::Phase::Stop )
    {
        for ( Paths::iterator it = _paths.begin(); it != _paths.end(); it++ )
        {
            (*it)->clear();
        }

        if ( _switchPaths->getNumChildren() > 0 )
        {
            _switchPaths->removeChildren( 0, _switchPaths->getNumChildren() );
        }
    }
    else if ( Data::get()->phase == Data::Phase::Work )
    {
        double azimuth = Data::get()->rotor.azimuth;
        double delta_psi = 2.0 * M_PI / (double)(MAX_BLADES);

        // main rotor blades
        for ( unsigned int i = 0; i < _paths.size() && i < MAX_BLADES; i++ )
        {
            double beta = -Data::get()->blade[ i ].beta;

            double psi = azimuth + i * delta_psi;
            double r = _rotorRadiusCW;

            if ( _direction == Data::Rotor::CCW )
            {
                //beta = -beta;
                psi = -psi;
                r = _rotorRadiusCCW;
            }

            osg::Quat q_sra( psi, osg::Z_AXIS );
            osg::Quat q_bsa( beta, osg::Y_AXIS );

            osg::Vec3 r_fh_sra(     -_bladesOffset, 0.0, 0.0 );
            osg::Vec3 r_tp_bsa( -r + _bladesOffset, 0.0, 0.0 );

            osg::Vec3 r_tp_ras = q_sra * r_fh_sra
                    + q_sra * ( q_bsa * r_tp_bsa );

            _paths[ i ]->push_back( r_tp_ras );

            double rot_time = ( 2.0 * M_PI ) / Data::get()->rotor.omega / Data::get()->time_coef;
            double frames = rot_time / CGI_TIME_STEP / MAX_BLADES;

            int max = ceil( frames );
            int min = 36;
            if ( max < min ) max = min;

            while ( _paths[ i ]->size() > (unsigned int)max )
            {
                _paths[ i ]->erase( _paths[ i ]->begin(),
                                    _paths[ i ]->begin() + 1 );
            }
        }
    }

    if ( Data::get()->other.show_blades_paths )
    {
        for ( Paths::iterator it = _paths.begin(); it != _paths.end(); it++ )
        {
            updatePath( _switchPaths.get(), (*it).get() );
        }

        _switchPaths->setAllChildrenOn();
    }
    else
    {
        _switchPaths->setAllChildrenOff();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Rotor::updatePath( osg::Group *parent, osg::Vec3Array *positions )
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    parent->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

    n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );
    c->push_back( osg::Vec4( 1.0f, 1.0f, 0.0f, 1.0f ) );

    geometry->setVertexArray( positions );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, 0, positions->size() ) );
    geometry->setNormalArray( n.get() );
    geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );
    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );

    osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth();
    lineWidth->setWidth( 2.0f );

    geode->getOrCreateStateSet()->setAttributeAndModes( lineWidth, osg::StateAttribute::ON );
}
