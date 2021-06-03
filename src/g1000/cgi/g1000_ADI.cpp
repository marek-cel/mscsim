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

#include <g1000/cgi/g1000_ADI.h>

#include <sstream>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LineWidth>
#include <osg/OccluderNode>

#include <osgText/Text>

#include <g1000/g1000_Defines.h>

#include <g1000/cgi/g1000_Colors.h>
#include <g1000/cgi/g1000_Fonts.h>

#include <g1000/utils/g1000_Misc.h>
#include <g1000/utils/g1000_Units.h>

////////////////////////////////////////////////////////////////////////////////

using namespace g1000;

////////////////////////////////////////////////////////////////////////////////

const double ADI::_x_offset = -10.0;
const double ADI::_y_offset =  19.0;

const osg::Vec3 ADI::_colorGround  = osg::Vec3( 0.33, 0.21, 0.04 );
const osg::Vec3 ADI::_colorHorizon = osg::Vec3( 0.27, 0.39, 0.89 );
const osg::Vec3 ADI::_colorSky     = osg::Vec3( 0.00, 0.30, 1.00 );

const double ADI::_z_aircraft_symbol = -80.0;
const double ADI::_z_horizon_line    = -100.0;
const double ADI::_z_pitch_scale     = -100.0;
const double ADI::_z_roll_scale      = -100.0;
const double ADI::_z_sky_and_ground  = -110.0;

const double ADI::_deg2pt = 1.35;
const double ADI::_rad2pt = ADI::_deg2pt * 180.0 / M_PI;

const double ADI::_fd_pitch_min = Units::deg2rad( -90.0 );
const double ADI::_fd_pitch_max = Units::deg2rad(  90.0 );

const double ADI::_max_slip_skid = Units::deg2rad( 10.0 );

const int ADI::_depth_sorted_bin_pitch_scale = 100;

////////////////////////////////////////////////////////////////////////////////

ADI::ADI( IFD *ifd ) :
    Module( ifd )
{
    _pat = new osg::PositionAttitudeTransform();
    _root->addChild( _pat.get() );
    _pat->setPosition( osg::Vec3( _x_offset, _y_offset, 0.0 ) );

    _patPitch = new osg::PositionAttitudeTransform();
    _pat->addChild( _patPitch.get() );

    _patRoll = new osg::PositionAttitudeTransform();
    _pat->addChild( _patRoll.get() );

    _patSlipSkid = new osg::PositionAttitudeTransform();
    _pat->addChild( _patSlipSkid.get() );

    _patHorizon = new osg::PositionAttitudeTransform();
    _pat->addChild( _patHorizon.get() );

    _switchFlightDir = new osg::Switch();
    _pat->addChild( _switchFlightDir.get() );

    _patFlightDir = new osg::PositionAttitudeTransform();
    _switchFlightDir->addChild( _patFlightDir.get() );

    createAircraftSymbol();
    createFlightDirector();
    createHorizonLine();
    createPitchScale();
    createPitchScaleMask();
    createRollPointer();
    createRollScale();
    createSkyAndGround();
}

////////////////////////////////////////////////////////////////////////////////

ADI::~ADI() {}

////////////////////////////////////////////////////////////////////////////////

void ADI::update()
{
    double ahrs_roll  = _ifd->grs()->getRoll();
    double ahrs_pitch = _ifd->grs()->getPitch();

    double fd_roll  = _ifd->input().fd_roll;
    double fd_pitch = _ifd->input().fd_pitch;
    bool fd_visible = _ifd->input().fd_visible;

    double ahrs_slipSkid = _ifd->grs()->getSlipSkid();

    double pitch = Misc::satur( -M_PI_2 , M_PI_2 , ahrs_pitch );
    double roll  = Misc::satur( -M_PI   , M_PI   , ahrs_roll  );

    double delta = rad2pt( pitch );

    double sinRoll = sin( roll );
    double cosRoll = cos( roll );

    double dx = delta * sinRoll;
    double dy = delta * cosRoll;

    osg::Quat q( roll, osg::Z_AXIS );
    osg::Vec3 r( dx, -dy, 0.0 );

    _patPitch->setAttitude( q );
    _patPitch->setPosition( r );

    _patRoll->setAttitude( q );

    double pitch_min = osg::DegreesToRadians( -30.0 );
    double pitch_max = osg::DegreesToRadians(  60.0 );

    double coef = 1.0 - 0.5 * ( cosRoll + 1.0 );

    pitch_min += coef * osg::DegreesToRadians( -30.0 );
    pitch_max += coef * osg::DegreesToRadians( -30.0 );

    pitch = Misc::satur( pitch_min, pitch_max, ahrs_pitch );

    delta = rad2pt( pitch );

    dx = delta * sinRoll;
    dy = delta * cosRoll;

    _patHorizon->setAttitude( q );
    _patHorizon->setPosition( osg::Vec3( dx, -dy, 0.0 ) );

    double slipSkid_norm = ahrs_slipSkid / _max_slip_skid;
    slipSkid_norm = Misc::satur( -1.0, 1.0, slipSkid_norm );
    _patSlipSkid->setPosition( osg::Vec3( 5.0 * -slipSkid_norm, 0.0, 0.0 ) );

    if ( fd_visible && fabs( ahrs_roll ) < Units::deg2rad( 65.0 )
      && ahrs_pitch < Units::deg2rad(  30.0 ) && ahrs_pitch > Units::deg2rad( -20.0 ) )
    {
        _switchFlightDir->setAllChildrenOn();

        double pitch = Misc::satur( -_fd_pitch_max, -_fd_pitch_min, ahrs_pitch - fd_pitch );
        double roll  = Misc::satur( -M_PI, M_PI, ahrs_roll - fd_roll  );

        double delta = rad2pt( pitch );

        double sinRoll = sin( roll );
        double cosRoll = cos( roll );

        double dx = delta * sinRoll;
        double dy = delta * cosRoll;

        osg::Quat q( roll, osg::Z_AXIS );
        osg::Vec3 r( dx, -dy, 0.0 );

        _patFlightDir->setAttitude( q );
        _patFlightDir->setPosition( r );
    }
    else
    {
        _switchFlightDir->setAllChildrenOff();
    }
}

////////////////////////////////////////////////////////////////////////////////

void ADI::createAircraftSymbol()
{
    const double zl = _z_aircraft_symbol;
    const double zd = _z_aircraft_symbol;
    const double zb = _z_aircraft_symbol + 1.0;

    const double dx_bars = 31.0;

    osg::ref_ptr<osg::Geode> geode_l = new osg::Geode();
    osg::ref_ptr<osg::Geode> geode_d = new osg::Geode();
    osg::ref_ptr<osg::Geode> geode_b = new osg::Geode();

    _pat->addChild( geode_l.get() );
    _pat->addChild( geode_d.get() );
    _pat->addChild( geode_b.get() );

    osg::ref_ptr<osg::Geometry> geom_l = new osg::Geometry();
    osg::ref_ptr<osg::Geometry> geom_d = new osg::Geometry();
    osg::ref_ptr<osg::Geometry> geom_b = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> vl = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec3Array> vd = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec3Array> vb = new osg::Vec3Array();

    osg::ref_ptr<osg::Vec4Array> cl = new osg::Vec4Array();
    osg::ref_ptr<osg::Vec4Array> cd = new osg::Vec4Array();
    osg::ref_ptr<osg::Vec4Array> cb = new osg::Vec4Array();

    vl->push_back( osg::Vec3(   0.0,  0.0, zl ) );
    vl->push_back( osg::Vec3( -17.0, -6.0, zl ) );
    vl->push_back( osg::Vec3( -22.0, -6.0, zl ) );
    vl->push_back( osg::Vec3(   0.0,  0.0, zl ) );
    vl->push_back( osg::Vec3(  17.0, -6.0, zl ) );
    vl->push_back( osg::Vec3(  22.0, -6.0, zl ) );

    vl->push_back( osg::Vec3( -dx_bars       ,  0.0, zl ) );
    vl->push_back( osg::Vec3( -dx_bars - 9.0 ,  0.0, zl ) );
    vl->push_back( osg::Vec3( -dx_bars - 1.0 ,  1.0, zl ) );
    vl->push_back( osg::Vec3( -dx_bars - 1.0 ,  1.0, zl ) );
    vl->push_back( osg::Vec3( -dx_bars - 9.0 ,  0.0, zl ) );
    vl->push_back( osg::Vec3( -dx_bars - 9.0 ,  1.0, zl ) );

    vl->push_back( osg::Vec3(  dx_bars       ,  0.0, zl ) );
    vl->push_back( osg::Vec3(  dx_bars + 9.0 ,  0.0, zl ) );
    vl->push_back( osg::Vec3(  dx_bars + 1.0 ,  1.0, zl ) );
    vl->push_back( osg::Vec3(  dx_bars + 1.0 ,  1.0, zl ) );
    vl->push_back( osg::Vec3(  dx_bars + 9.0 ,  0.0, zl ) );
    vl->push_back( osg::Vec3(  dx_bars + 9.0 ,  1.0, zl ) );

    cl->push_back( osg::Vec4( Colors::_yellow, 1.0 ) );

    vd->push_back( osg::Vec3(   0.0,  0.0, zd ) );
    vd->push_back( osg::Vec3( -12.0, -6.0, zd ) );
    vd->push_back( osg::Vec3( -17.0, -6.0, zd ) );
    vd->push_back( osg::Vec3(   0.0,  0.0, zd ) );
    vd->push_back( osg::Vec3(  12.0, -6.0, zd ) );
    vd->push_back( osg::Vec3(  17.0, -6.0, zd ) );

    vd->push_back( osg::Vec3( -dx_bars       ,  0.0, zd ) );
    vd->push_back( osg::Vec3( -dx_bars - 9.0 ,  0.0, zd ) );
    vd->push_back( osg::Vec3( -dx_bars - 1.0 , -1.0, zd ) );
    vd->push_back( osg::Vec3( -dx_bars - 1.0 , -1.0, zd ) );
    vd->push_back( osg::Vec3( -dx_bars - 9.0 ,  0.0, zd ) );
    vd->push_back( osg::Vec3( -dx_bars - 9.0 , -1.0, zd ) );

    vd->push_back( osg::Vec3(  dx_bars       ,  0.0, zd ) );
    vd->push_back( osg::Vec3(  dx_bars + 9.0 ,  0.0, zd ) );
    vd->push_back( osg::Vec3(  dx_bars + 1.0 , -1.0, zd ) );
    vd->push_back( osg::Vec3(  dx_bars + 1.0 , -1.0, zd ) );
    vd->push_back( osg::Vec3(  dx_bars + 9.0 ,  0.0, zd ) );
    vd->push_back( osg::Vec3(  dx_bars + 9.0 , -1.0, zd ) );

    cd->push_back( osg::Vec4( Colors::_olive, 1.0 ) );

    vb->push_back( osg::Vec3(   0.0,  0.0, zb ) );
    vb->push_back( osg::Vec3( -12.0, -6.0, zb ) );
    vb->push_back( osg::Vec3( -12.0, -6.0, zb ) );
    vb->push_back( osg::Vec3( -22.0, -6.0, zb ) );
    vb->push_back( osg::Vec3( -22.0, -6.0, zb ) );
    vb->push_back( osg::Vec3(   0.0,  0.0, zb ) );
    vb->push_back( osg::Vec3(   0.0,  0.0, zb ) );
    vb->push_back( osg::Vec3(  12.0, -6.0, zb ) );
    vb->push_back( osg::Vec3(  12.0, -6.0, zb ) );
    vb->push_back( osg::Vec3(  22.0, -6.0, zb ) );
    vb->push_back( osg::Vec3(  22.0, -6.0, zb ) );
    vb->push_back( osg::Vec3(   0.0,  0.0, zb ) );

    vb->push_back( osg::Vec3( -dx_bars       ,  0.0, zb ) );
    vb->push_back( osg::Vec3( -dx_bars - 1.0 , -1.0, zb ) );
    vb->push_back( osg::Vec3( -dx_bars - 1.0 , -1.0, zb ) );
    vb->push_back( osg::Vec3( -dx_bars - 9.0 , -1.0, zb ) );
    vb->push_back( osg::Vec3( -dx_bars - 9.0 , -1.0, zb ) );
    vb->push_back( osg::Vec3( -dx_bars - 9.0 ,  1.0, zb ) );
    vb->push_back( osg::Vec3( -dx_bars - 9.0 ,  1.0, zb ) );
    vb->push_back( osg::Vec3( -dx_bars - 1.0 ,  1.0, zb ) );
    vb->push_back( osg::Vec3( -dx_bars - 1.0 ,  1.0, zb ) );
    vb->push_back( osg::Vec3( -dx_bars       ,  0.0, zb ) );

    vb->push_back( osg::Vec3(  dx_bars       ,  0.0, zb ) );
    vb->push_back( osg::Vec3(  dx_bars + 1.0 , -1.0, zb ) );
    vb->push_back( osg::Vec3(  dx_bars + 1.0 , -1.0, zb ) );
    vb->push_back( osg::Vec3(  dx_bars + 9.0 , -1.0, zb ) );
    vb->push_back( osg::Vec3(  dx_bars + 9.0 , -1.0, zb ) );
    vb->push_back( osg::Vec3(  dx_bars + 9.0 ,  1.0, zb ) );
    vb->push_back( osg::Vec3(  dx_bars + 9.0 ,  1.0, zb ) );
    vb->push_back( osg::Vec3(  dx_bars + 1.0 ,  1.0, zb ) );
    vb->push_back( osg::Vec3(  dx_bars + 1.0 ,  1.0, zb ) );
    vb->push_back( osg::Vec3(  dx_bars       ,  0.0, zb ) );

    cb->push_back( osg::Vec4( Colors::_black, 1.0 ) );

    geom_l->setVertexArray( vl.get() );
    geom_l->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLES, 0, vl->size() ) );

    geom_l->setColorArray( cl.get() );
    geom_l->setColorBinding( osg::Geometry::BIND_OVERALL );

    geom_d->setVertexArray( vd.get() );
    geom_d->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLES, 0, vd->size() ) );

    geom_d->setColorArray( cd.get() );
    geom_d->setColorBinding( osg::Geometry::BIND_OVERALL );

    geom_b->setVertexArray( vb.get() );
    geom_b->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, vb->size() ) );

    geom_b->setColorArray( cb.get() );
    geom_b->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode_l->addDrawable( geom_l.get() );
    geode_d->addDrawable( geom_d.get() );
    geode_b->addDrawable( geom_b.get() );
}

////////////////////////////////////////////////////////////////////////////////

void ADI::createFlightDirector()
{
    const double zf = _z_aircraft_symbol + 1.0;
    const double zb = _z_aircraft_symbol + 2.0;

    osg::ref_ptr<osg::Geode> geode_f = new osg::Geode();
    osg::ref_ptr<osg::Geode> geode_b = new osg::Geode();

    _patFlightDir->addChild( geode_f.get() );
    _patFlightDir->addChild( geode_b.get() );

    osg::ref_ptr<osg::Geometry> geom_f = new osg::Geometry();
    osg::ref_ptr<osg::Geometry> geom_b = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> vf = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec3Array> vb = new osg::Vec3Array();

    osg::ref_ptr<osg::Vec4Array> cf = new osg::Vec4Array();
    osg::ref_ptr<osg::Vec4Array> cb = new osg::Vec4Array();

    vf->push_back( osg::Vec3(   0.0,  0.0, zf ) );
    vf->push_back( osg::Vec3( -22.0, -6.0, zf ) );
    vf->push_back( osg::Vec3( -26.0, -3.5, zf ) );
    vf->push_back( osg::Vec3( -22.0, -6.0, zf ) );
    vf->push_back( osg::Vec3( -26.0, -3.5, zf ) );
    vf->push_back( osg::Vec3( -27.0, -6.0, zf ) );
    vf->push_back( osg::Vec3(   0.0,  0.0, zf ) );
    vf->push_back( osg::Vec3(  22.0, -6.0, zf ) );
    vf->push_back( osg::Vec3(  26.0, -3.5, zf ) );
    vf->push_back( osg::Vec3(  22.0, -6.0, zf ) );
    vf->push_back( osg::Vec3(  26.0, -3.5, zf ) );
    vf->push_back( osg::Vec3(  27.0, -6.0, zf ) );

    cf->push_back( osg::Vec4( Colors::_magenta, 1.0 ) );

    vb->push_back( osg::Vec3(   0.0,  0.0, zb ) );
    vb->push_back( osg::Vec3( -22.0, -6.0, zb ) );
    vb->push_back( osg::Vec3( -22.0, -6.0, zb ) );
    vb->push_back( osg::Vec3( -26.0, -3.5, zb ) );
    vb->push_back( osg::Vec3( -26.0, -3.5, zb ) );
    vb->push_back( osg::Vec3(   0.0,  0.0, zb ) );
    vb->push_back( osg::Vec3( -22.0, -6.0, zb ) );
    vb->push_back( osg::Vec3( -27.0, -6.0, zb ) );
    vb->push_back( osg::Vec3( -27.0, -6.0, zb ) );
    vb->push_back( osg::Vec3( -26.0, -3.5, zf ) );

    vb->push_back( osg::Vec3(   0.0,  0.0, zb ) );
    vb->push_back( osg::Vec3(  22.0, -6.0, zb ) );
    vb->push_back( osg::Vec3(  22.0, -6.0, zb ) );
    vb->push_back( osg::Vec3(  26.0, -3.5, zb ) );
    vb->push_back( osg::Vec3(  26.0, -3.5, zb ) );
    vb->push_back( osg::Vec3(   0.0,  0.0, zb ) );
    vb->push_back( osg::Vec3(  22.0, -6.0, zb ) );
    vb->push_back( osg::Vec3(  27.0, -6.0, zb ) );
    vb->push_back( osg::Vec3(  27.0, -6.0, zb ) );
    vb->push_back( osg::Vec3(  26.0, -3.5, zf ) );

    cb->push_back( osg::Vec4( Colors::_black, 1.0 ) );

    geom_f->setVertexArray( vf.get() );
    geom_f->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLES, 0, vf->size() ) );

    geom_f->setColorArray( cf.get() );
    geom_f->setColorBinding( osg::Geometry::BIND_OVERALL );

    geom_b->setVertexArray( vb.get() );
    geom_b->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, vb->size() ) );

    geom_b->setColorArray( cb.get() );
    geom_b->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode_f->addDrawable( geom_f.get() );
    geode_b->addDrawable( geom_b.get() );
}

////////////////////////////////////////////////////////////////////////////////

void ADI::createHorizonLine()
{
    const double d = 150.0;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _patHorizon->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    v->push_back( osg::Vec3( -d, 0.0, _z_horizon_line ) );
    v->push_back( osg::Vec3(  d, 0.0, _z_horizon_line ) );

    c->push_back( osg::Vec4( Colors::_white, 1.0 ) );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );

    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );
}

////////////////////////////////////////////////////////////////////////////////

void ADI::createPitchScale()
{
    osg::ref_ptr<osg::Geode> geode_s = new osg::Geode();
    osg::ref_ptr<osg::Geode> geode_a = new osg::Geode();

    _patPitch->addChild( geode_s.get() );
    _patPitch->addChild( geode_a.get() );

    osg::ref_ptr<osg::Geometry> geom_s = new osg::Geometry();
    osg::ref_ptr<osg::Geometry> geom_a = new osg::Geometry();

    osg::ref_ptr<osg::StateSet> stateSet_s = geode_s->getOrCreateStateSet();
    osg::ref_ptr<osg::StateSet> stateSet_a = geode_a->getOrCreateStateSet();

    stateSet_s->setRenderBinDetails( _depth_sorted_bin_pitch_scale, "RenderBin" );
    stateSet_a->setRenderBinDetails( _depth_sorted_bin_pitch_scale, "RenderBin" );

    osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth();
    lineWidth->setWidth( 6.0f );
    stateSet_a->setAttributeAndModes( lineWidth, osg::StateAttribute::ON );

    osg::ref_ptr<osg::Vec3Array> vs = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array();

    osg::ref_ptr<osg::Vec4Array> cs = new osg::Vec4Array();
    osg::ref_ptr<osg::Vec4Array> ca = new osg::Vec4Array();

    cs->push_back( osg::Vec4( Colors::_white , 1.0 ) );
    ca->push_back( osg::Vec4( Colors::_red   , 1.0 ) );

    for ( int i = 0; i <= 64; i++ )
    {
        double pitch = -80.0 + i * 2.5;

        if ( i % 2 == 0 || ( -20.0 < pitch && pitch < 20.0 ) )
        {
            double width = 5.0;
            bool label = false;

            if ( i % 4 == 0 )
            {
                width = 20.0;
                label = i != 32;

                if ( pitch < -20.01 || 40.01 < pitch )
                {
                    const double w = width / 2.0;
                    const double y = deg2pt( pitch );
                    const double h = pitch < 0.0 ? -deg2pt( 10.0 ) : deg2pt( 10.0 );

                    va->push_back( osg::Vec3(  -w, y + h , _z_pitch_scale + 1.0 ) );
                    va->push_back( osg::Vec3( 0.0, y     , _z_pitch_scale + 1.0 ) );
                    va->push_back( osg::Vec3( 0.0, y     , _z_pitch_scale + 1.0 ) );
                    va->push_back( osg::Vec3(   w, y + h , _z_pitch_scale + 1.0 ) );
                }
            }
            else if ( i % 2 == 0 )
            {
                width = 10.0;
            }

            createPitchScaleBar( geode_s.get(), vs.get(), pitch, width, label );
        }
    }

    geom_s->setVertexArray( vs.get() );
    geom_a->setVertexArray( va.get() );

    geom_s->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, vs->size() ) );
    geom_a->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, va->size() ) );

    geom_s->setColorArray( cs.get() );
    geom_a->setColorArray( ca.get() );

    geom_s->setColorBinding( osg::Geometry::BIND_OVERALL );
    geom_a->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode_s->addDrawable( geom_s.get() );
    geode_a->addDrawable( geom_a.get() );
}

////////////////////////////////////////////////////////////////////////////////

void ADI::createPitchScaleBar( osg::Geode *geode, osg::Vec3Array *v,
                                   double pitch, double width, bool label )
{
    const double w = width / 2.0;
    const double y = deg2pt( pitch );

    v->push_back( osg::Vec3( -w, y, _z_pitch_scale ) );
    v->push_back( osg::Vec3(  w, y, _z_pitch_scale ) );

    if ( label )
    {
        std::stringstream pitch_str;
        pitch_str << ( pitch < 0 ? -pitch : pitch );

        // text L
        {
            osg::ref_ptr<osgText::Text> text = new osgText::Text();
            text->setFont( Fonts::get( "fonts/g1000.ttf" ) );
            text->setColor( osg::Vec4( Colors::_white, 1.0 ) );
            text->setCharacterSize( 4.0 );
            text->setAxisAlignment( osgText::TextBase::XY_PLANE );
            text->setPosition( osg::Vec3( -w - 1.0, y, _z_pitch_scale ) );
            text->setLayout( osgText::Text::LEFT_TO_RIGHT );
            text->setAlignment( osgText::Text::RIGHT_CENTER );
            text->setText( pitch_str.str() );
            geode->addDrawable( text );
        }

        // text R
        {
            osg::ref_ptr<osgText::Text> text = new osgText::Text();
            text->setFont( Fonts::get( "fonts/g1000.ttf" ) );
            text->setColor( osg::Vec4( Colors::_white, 1.0 ) );
            text->setCharacterSize( 4.0 );
            text->setAxisAlignment( osgText::TextBase::XY_PLANE );
            text->setPosition( osg::Vec3( w + 1.0, y, _z_pitch_scale ) );
            text->setLayout( osgText::Text::LEFT_TO_RIGHT );
            text->setAlignment( osgText::Text::LEFT_CENTER );
            text->setText( pitch_str.str() );
            geode->addDrawable( text );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void ADI::createPitchScaleMask()
{
    osg::ref_ptr<osg::Geode> geode_f = new osg::Geode();
    osg::ref_ptr<osg::Geode> geode_m = new osg::Geode();

    _pat->addChild( geode_f.get() );
    _patRoll->addChild( geode_m.get() );

    osg::ref_ptr<osg::StateSet> stateSet_f = geode_f->getOrCreateStateSet();
    osg::ref_ptr<osg::StateSet> stateSet_m = geode_m->getOrCreateStateSet();

    stateSet_f->setRenderBinDetails( _depth_sorted_bin_pitch_scale - 1, "RenderBin" );
    stateSet_m->setRenderBinDetails( _depth_sorted_bin_pitch_scale - 1, "RenderBin" );

    osg::ref_ptr<osg::Geometry> geom_f = new osg::Geometry();
    osg::ref_ptr<osg::Geometry> geom_m = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    c->push_back( osg::Vec4( 0.0, 0.0, 0.0, 0.0 ) );

    {
        const double h = 150.0;
        const double w = 31.0;
        const double t = 29.0;

        v->push_back( osg::Vec3( -w, t     , _z_pitch_scale + 1.0 ) );
        v->push_back( osg::Vec3(  w, t     , _z_pitch_scale + 1.0 ) );
        v->push_back( osg::Vec3(  w, t + h , _z_pitch_scale + 1.0 ) );
        v->push_back( osg::Vec3( -w, t + h , _z_pitch_scale + 1.0 ) );
    }

    geom_m->setVertexArray( v.get() );
    geom_m->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, v->size() ) );

    geom_m->setColorArray( c.get() );
    geom_m->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode_m->addDrawable( geom_m.get() );

    {
        double x = -G1000_GDU_WIDTH_2  - _x_offset;
        double w =  G1000_GDU_WIDTH;
        double y = -21.0;
        double h =  G1000_GDU_HEIGHT_2 + _y_offset + y;

        v->push_back( osg::Vec3( x     , y - h , _z_pitch_scale + 1.0 ) );
        v->push_back( osg::Vec3( x + w , y - h , _z_pitch_scale + 1.0 ) );
        v->push_back( osg::Vec3( x + w , y     , _z_pitch_scale + 1.0 ) );
        v->push_back( osg::Vec3( x     , y     , _z_pitch_scale + 1.0 ) );
    }

    {
        double x = -31;
        double w =  G1000_GDU_WIDTH_2  + x + _x_offset;
        double y = -21.0;
        double h =  G1000_GDU_HEIGHT_2 - _y_offset - y;

        v->push_back( osg::Vec3( x - w , y     , _z_pitch_scale + 1.0 ) );
        v->push_back( osg::Vec3( x     , y     , _z_pitch_scale + 1.0 ) );
        v->push_back( osg::Vec3( x     , y + h , _z_pitch_scale + 1.0 ) );
        v->push_back( osg::Vec3( x - w , y + h , _z_pitch_scale + 1.0 ) );
    }

    {
        double x =  31;
        double w =  G1000_GDU_WIDTH_2  - x - _x_offset;
        double y = -21.0;
        double h =  G1000_GDU_HEIGHT_2 - _y_offset - y;

        v->push_back( osg::Vec3( x     , y     , _z_pitch_scale + 1.0 ) );
        v->push_back( osg::Vec3( x + w , y     , _z_pitch_scale + 1.0 ) );
        v->push_back( osg::Vec3( x + w , y + h , _z_pitch_scale + 1.0 ) );
        v->push_back( osg::Vec3( x     , y + h , _z_pitch_scale + 1.0 ) );
    }

    geom_f->setVertexArray( v.get() );
    geom_f->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, v->size() ) );

    geom_f->setColorArray( c.get() );
    geom_f->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode_f->addDrawable( geom_f.get() );
}

////////////////////////////////////////////////////////////////////////////////

void ADI::createRollPointer()
{
    const double r0 = 37.0;

    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();
    c->push_back( osg::Vec4( Colors::_white, 1.0 ) );

    // roll pointer
    {
        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        _pat->addChild( geode.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();

        v->push_back( osg::Vec3(  0.0, r0       , _z_roll_scale ) );
        v->push_back( osg::Vec3( -2.0, r0 - 4.0 , _z_roll_scale ) );
        v->push_back( osg::Vec3(  2.0, r0 - 4.0 , _z_roll_scale ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLES, 0, v->size() ) );

        geometry->setColorArray( c.get() );
        geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }

    // slip skid indicator
    {
        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        _patSlipSkid->addChild( geode.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();

        v->push_back( osg::Vec3( -2.396, r0 - 4.792, _z_roll_scale ) );
        v->push_back( osg::Vec3( -2.883, r0 - 5.767, _z_roll_scale ) );
        v->push_back( osg::Vec3(  2.883, r0 - 5.767, _z_roll_scale ) );
        v->push_back( osg::Vec3(  2.396, r0 - 4.792, _z_roll_scale ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, v->size() ) );

        geometry->setColorArray( c.get() );
        geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void ADI::createRollScale()
{
    const double ll = 6.0;
    const double ls = 3.0;

    const double r0 = 37.0;
    const double r1 = r0 + ls;
    const double r2 = r0 + ll;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _patRoll->addChild( geode.get() );

    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();
    c->push_back( osg::Vec4( Colors::_white, 1.0 ) );

    // arc
    {
        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();

        const double step = 5.0;
        const double min = -60.0;
        const double max =  60.0;
        const int steps = ( max - min ) / step + 1;

        for ( int i = 0; i < steps; i++ )
        {
            double a = osg::DegreesToRadians( min + i * step );

            double x = r0 * sin( a );
            double y = r0 * cos( a );

            v->push_back( osg::Vec3( x, y, _z_roll_scale ) );
        }

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, 0, v->size() ) );

        geometry->setColorArray( c.get() );
        geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }

    // scale
    {
        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();

        double a = 0.0;

        double x0 = 0.0;
        double x1 = 0.0;
        double x2 = 0.0;

        double y0 = 0.0;
        double y1 = 0.0;
        double y2 = 0.0;

        // -60
        a = osg::DegreesToRadians( -60.0 );

        x0 = r0 * sin( a );
        x1 = r2 * sin( a );

        y0 = r0 * cos( a );
        y1 = r2 * cos( a );

        v->push_back( osg::Vec3( -x0, y0, _z_roll_scale ) );
        v->push_back( osg::Vec3( -x1, y1, _z_roll_scale ) );

        v->push_back( osg::Vec3(  x0, y0, _z_roll_scale ) );
        v->push_back( osg::Vec3(  x1, y1, _z_roll_scale ) );

        // -45
        a = osg::DegreesToRadians( -45.0 );

        x0 = r0 * sin( a );
        x1 = r1 * sin( a );

        y0 = r0 * cos( a );
        y1 = r1 * cos( a );

        v->push_back( osg::Vec3( -x0, y0, _z_roll_scale ) );
        v->push_back( osg::Vec3( -x1, y1, _z_roll_scale ) );

        v->push_back( osg::Vec3(  x0, y0, _z_roll_scale ) );
        v->push_back( osg::Vec3(  x1, y1, _z_roll_scale ) );

        // -30
        a = osg::DegreesToRadians( -30.0 );

        x0 = r0 * sin( a );
        x2 = r2 * sin( a );

        y0 = r0 * cos( a );
        y2 = r2 * cos( a );

        v->push_back( osg::Vec3( -x0, y0, _z_roll_scale ) );
        v->push_back( osg::Vec3( -x2, y2, _z_roll_scale ) );

        v->push_back( osg::Vec3(  x0, y0, _z_roll_scale ) );
        v->push_back( osg::Vec3(  x2, y2, _z_roll_scale ) );

        // -20
        a = osg::DegreesToRadians( -20.0 );

        x0 = r0 * sin( a );
        x1 = r1 * sin( a );

        y0 = r0 * cos( a );
        y1 = r1 * cos( a );

        v->push_back( osg::Vec3( -x0, y0, _z_roll_scale ) );
        v->push_back( osg::Vec3( -x1, y1, _z_roll_scale ) );

        v->push_back( osg::Vec3(  x0, y0, _z_roll_scale ) );
        v->push_back( osg::Vec3(  x1, y1, _z_roll_scale ) );

        // -10
        a = osg::DegreesToRadians( -10.0 );

        x0 = r0 * sin( a );
        x1 = r1 * sin( a );

        y0 = r0 * cos( a );
        y1 = r1 * cos( a );

        v->push_back( osg::Vec3( -x0, y0, _z_roll_scale ) );
        v->push_back( osg::Vec3( -x1, y1, _z_roll_scale ) );

        v->push_back( osg::Vec3(  x0, y0, _z_roll_scale ) );
        v->push_back( osg::Vec3(  x1, y1, _z_roll_scale ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );

        geometry->setColorArray( c.get() );
        geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }

    // marker
    {
        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();

        v->push_back( osg::Vec3(  0.0, r0       , _z_roll_scale ) );
        v->push_back( osg::Vec3( -2.0, r0 + 4.0 , _z_roll_scale ) );
        v->push_back( osg::Vec3(  2.0, r0 + 4.0 , _z_roll_scale ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLES, 0, v->size() ) );

        geometry->setColorArray( c.get() );
        geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void ADI::createSkyAndGround()
{
    const double d = 150.0;
    const double t = deg2pt( 20.0 );
    const double h = deg2pt( 90.0 ) + d;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _patPitch->addChild( geode.get() );

    // sky
    {
        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
        osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

        v->push_back( osg::Vec3( -d, 0.0, _z_sky_and_ground ) );
        v->push_back( osg::Vec3(  d, 0.0, _z_sky_and_ground ) );
        v->push_back( osg::Vec3( -d,   t, _z_sky_and_ground ) );
        v->push_back( osg::Vec3(  d,   t, _z_sky_and_ground ) );
        v->push_back( osg::Vec3( -d,   h, _z_sky_and_ground ) );
        v->push_back( osg::Vec3(  d,   h, _z_sky_and_ground ) );

        c->push_back( osg::Vec4( _colorHorizon , 1.0 ) );
        c->push_back( osg::Vec4( _colorHorizon , 1.0 ) );
        c->push_back( osg::Vec4( _colorSky     , 1.0 ) );
        c->push_back( osg::Vec4( _colorSky     , 1.0 ) );
        c->push_back( osg::Vec4( _colorSky     , 1.0 ) );
        c->push_back( osg::Vec4( _colorSky     , 1.0 ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUAD_STRIP, 0, v->size() ) );

        geometry->setColorArray( c.get() );
        geometry->setColorBinding( osg::Geometry::BIND_PER_VERTEX );

        geode->addDrawable( geometry.get() );
    }

    // ground
    {
        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
        osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

        v->push_back( osg::Vec3( -d,  0.0, _z_sky_and_ground ) );
        v->push_back( osg::Vec3(  d,  0.0, _z_sky_and_ground ) );
        v->push_back( osg::Vec3(  d,   -h, _z_sky_and_ground ) );
        v->push_back( osg::Vec3( -d,   -h, _z_sky_and_ground ) );

        c->push_back( osg::Vec4( _colorGround, 1.0 ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, v->size() ) );

        geometry->setColorArray( c.get() );
        geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }
}
