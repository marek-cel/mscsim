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

#include <cgi/otw/cgi_Rotor.h>

#include <osg/AlphaFunc>
#include <osg/BlendFunc>
#include <osg/Depth>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LineWidth>

#include <fdm/utils/fdm_Units.h>

#include <Data.h>

#include <cgi/cgi_Defines.h>
#include <cgi/cgi_Utils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Rotor::Rotor( const osg::Vec3 &center, int blades_count,
              double radius, double offset, double inclination ) :
    _center ( center ),
    _blades_count ( blades_count ),
    _radius ( radius ),
    _offset ( offset ),
    _inclination ( inclination )
{
    _root = new osg::Group();

    _pat = new osg::PositionAttitudeTransform();
    _root->addChild( _pat.get() );

    _pat->setPosition( _center );
    _pat->setAttitude( osg::Quat( -_inclination, osg::Y_AXIS ) );

    _switchBlur = new osg::Switch();
    _pat->addChild( _switchBlur.get() );

    _switchDatum = new osg::Switch();
    _pat->addChild( _switchDatum.get() );

    _patDatum = new osg::PositionAttitudeTransform();
    _switchDatum->addChild( _patDatum.get() );

    _switchPaths = new osg::Switch();
    _pat->addChild( _switchPaths.get() );

    _switchVectors = new osg::Switch();
    _pat->addChild( _switchVectors.get() );

    for ( int i = 0; i < _blades_count; i++ )
    {
        _paths.push_back( new osg::Vec3Array() );
    }

    createDatum();
}

////////////////////////////////////////////////////////////////////////////////

Rotor::~Rotor() {}

////////////////////////////////////////////////////////////////////////////////

void Rotor::update()
{
    updateBlur();
    updateDatum();
    updatePaths();
    updateVectors();
}

////////////////////////////////////////////////////////////////////////////////

void Rotor::createDatum()
{
    const double r = _radius;

    // ring
    {
        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        _switchDatum->addChild( geode.get() );

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
        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        _switchDatum->addChild( geode.get() );

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

    for ( int i = 0; i < _blades_count; i++ )
    {
        double psi = i * ( 2.0 * M_PI / (double)_blades_count );

        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        _patDatum->addChild( geode.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // normals
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
        osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

        v->push_back( osg::Vec3( 0.0f, 0.0f, 0.0f ) );
        v->push_back( osg::Vec3( sin( psi ) * _radius, cos( psi ) * _radius, 0.0 ) );

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

////////////////////////////////////////////////////////////////////////////////

void Rotor::updateBlur()
{
    if ( _switchBlur->getNumChildren() > 0 )
    {
        _switchBlur->removeChildren( 0, _switchBlur->getNumChildren() );
    }

    if ( Data::get()->cgi.show_rotor_blur )
    {
        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        _switchBlur->addChild( geode.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
        geode->addDrawable( geometry.get() );

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();
        osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

        const double span = _radius - _offset;

        for ( int i = 0; i <= 36; i++ )
        {
            double psi = fdm::Units::deg2rad( 10.0 * i );

            double cosPsi = cos( psi );
            double sinPsi = sin( psi );

            double beta = Data::get()->ownship.mainRotor.coningAngle
                        + cosPsi * Data::get()->ownship.mainRotor.diskPitch
                        - sinPsi * Data::get()->ownship.mainRotor.diskRoll;

            double cosBeta = cos( beta );
            double sinBeta = sin( beta );

            double span_real = cosBeta * span;

            double xi = cosPsi * _offset;
            double xo = cosPsi * ( _offset + span_real );

            double yi = sinPsi * _offset;
            double yo = sinPsi * ( _offset + span_real );

            double zo = sinBeta * span;

            v->push_back( osg::Vec3( xi, yi, 0.0 ) );
            v->push_back( osg::Vec3( xo, yo, -zo ) );
        }

        n->push_back( osg::Vec3( 0.0f, 0.0f, -1.0f ) );
        c->push_back( osg::Vec4( 0.0f, 0.0f, 0.0f, 0.2f ) );

        geometry->setVertexArray( v );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUAD_STRIP, 0, v->size() ) );
        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );
        geometry->setColorArray( c.get() );
        geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

        osg::ref_ptr<osg::StateSet> stateSet = _root->getOrCreateStateSet();

        osg::ref_ptr<osg::AlphaFunc> alphaFunc = new osg::AlphaFunc();
        osg::ref_ptr<osg::BlendFunc> blendFunc = new osg::BlendFunc();
        alphaFunc->setFunction( osg::AlphaFunc::GEQUAL, 0.05f );
        blendFunc->setFunction( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        osg::ref_ptr<osg::Depth> depth = new osg::Depth;
        depth->setWriteMask( false );
        stateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
        stateSet->setAttributeAndModes( blendFunc.get(), osg::StateAttribute::ON );
        stateSet->setAttributeAndModes( alphaFunc.get(), osg::StateAttribute::ON );
        stateSet->setAttributeAndModes( depth.get(), osg::StateAttribute::ON );
        stateSet->setMode( GL_BLEND     , osg::StateAttribute::ON  | osg::StateAttribute::OVERRIDE );
        stateSet->setMode( GL_CULL_FACE , osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE  );
        stateSet->setRenderBinDetails( CGI_DEPTH_SORTED_BIN_EFFECTS, "DepthSortedBin" );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Rotor::updateDatum()
{
    if ( Data::get()->cgi.show_blades_datum )
    {
        _switchDatum->setAllChildrenOn();

        double psi = Data::get()->ownship.mainRotor.coef * Data::get()->ownship.mainRotor.azimuth;
        _patDatum->setAttitude( osg::Quat( psi, osg::Z_AXIS ) );
    }
    else
    {
        _switchDatum->setAllChildrenOff();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Rotor::updatePaths()
{
    if ( _switchPaths->getNumChildren() > 0 )
    {
        _switchPaths->removeChildren( 0, _switchPaths->getNumChildren() );
    }

    if ( Data::get()->stateOut == fdm::DataOut::Idle )
    {
        for ( Paths::iterator it = _paths.begin(); it != _paths.end(); ++it )
        {
            (*it)->clear();
        }

        if ( _switchPaths->getNumChildren() > 0 )
        {
            _switchPaths->removeChildren( 0, _switchPaths->getNumChildren() );
        }
    }
    else if ( Data::get()->stateOut == fdm::DataOut::Working )
    {
        double azimuth = Data::get()->ownship.mainRotor.azimuth;
        double delta_psi = 2.0 * M_PI / (double)(_blades_count);

        // main rotor blades
        for ( int i = 0; i < (int)_paths.size() && i < _blades_count; i++ )
        {
            double beta = -Data::get()->ownship.mainRotor.blade[ i ].flapping;

            double psi = -azimuth + i * delta_psi * Data::get()->ownship.mainRotor.coef;
            double r = _radius;

            osg::Quat q_sra( psi  , osg::Z_AXIS );
            osg::Quat q_bsa( beta , osg::Y_AXIS );

            osg::Vec3 r_fh_sra(     -_offset, 0.0, 0.0 );
            osg::Vec3 r_tp_bsa( -r + _offset, 0.0, 0.0 );

            osg::Vec3 r_tp_ras = q_sra * r_fh_sra
                    + q_sra * ( q_bsa * r_tp_bsa );

            _paths[ i ]->push_back( r_tp_ras );

            double rot_time = ( 2.0 * M_PI ) / Data::get()->ownship.mainRotor.omega / Data::get()->timeCoef;
            double frames = rot_time / CGI_TIME_STEP / (double)_blades_count;

            int max = ceil( frames );
            int min = 10;
            if ( max < min ) max = min;

            while ( _paths[ i ]->size() > (unsigned int)max )
            {
                _paths[ i ]->erase( _paths[ i ]->begin(),
                                    _paths[ i ]->begin() + 1 );
            }
        }
    }

    if ( Data::get()->cgi.show_blades_paths )
    {
        for ( Paths::iterator it = _paths.begin(); it != _paths.end(); ++it )
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

////////////////////////////////////////////////////////////////////////////////

void Rotor::updateVectors()
{
    if ( _switchVectors->getNumChildren() > 0 )
    {
        _switchVectors->removeChildren( 0, _switchVectors->getNumChildren() );
    }

    for ( int i = 0; i < FDM_MAX_VECTORS; i++ )
    {
        if ( Data::get()->cgi.rotor.v[ i ].visible )
        {
            osg::Vec3 b( Data::get()->cgi.rotor.v[ i ].b_x,
                         Data::get()->cgi.rotor.v[ i ].b_y,
                         Data::get()->cgi.rotor.v[ i ].b_z );

            osg::Vec3 v( Data::get()->cgi.rotor.v[ i ].v_x,
                         Data::get()->cgi.rotor.v[ i ].v_y,
                         Data::get()->cgi.rotor.v[ i ].v_z );

            Utils::drawVector( _switchVectors.get(), b, v );
        }
    }
}

