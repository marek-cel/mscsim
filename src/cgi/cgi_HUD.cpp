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
#include <cgi/cgi_HUD.h>

#include <stdio.h>

#include <osg/Drawable>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Light>
#include <osg/LineWidth>
#include <osg/Material>
#include <osg/PositionAttitudeTransform>
#include <osg/Switch>

#include <osgText/Text>

#include <fdmUtils/fdm_Units.h>

#include <Data.h>

#include <cgi/cgi_Color.h>
#include <cgi/cgi_Defines.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

const float HUD::m_charSize  = 5.0f;
const float HUD::m_angleCoef = 600.0f / 90.0f;
const float HUD::m_rollLimit = osg::DegreesToRadians( 75.0 );

////////////////////////////////////////////////////////////////////////////////

HUD::HUD()
{
    m_root = new osg::Group();

    m_switch = new osg::Switch();
    m_root->addChild( m_switch.get() );

    createWaterLine();

    createRollIndicator();
    createPitchLadder();
    createHeadingScale();

    createTextL();
    createTextR();

    createFPM();
    createILS();

    createStall();

    osg::ref_ptr<osg::StateSet> stateSet = m_root->getOrCreateStateSet();

    stateSet->setMode( GL_RESCALE_NORMAL , osg::StateAttribute::ON  );
    stateSet->setMode( GL_LIGHTING       , osg::StateAttribute::OFF );
    stateSet->setMode( GL_LIGHT0         , osg::StateAttribute::ON  );
    stateSet->setMode( GL_BLEND          , osg::StateAttribute::ON  );
    stateSet->setMode( GL_ALPHA_TEST     , osg::StateAttribute::ON  );
    stateSet->setMode( GL_DEPTH_TEST     , osg::StateAttribute::ON  );
    stateSet->setMode( GL_DITHER         , osg::StateAttribute::OFF );
    stateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
    stateSet->setRenderBinDetails( CGI_DEPTH_SORTED_BIN_HUD, "RenderBin" );

    osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth();
    lineWidth->setWidth( 2.0f );

    stateSet->setAttributeAndModes( lineWidth, osg::StateAttribute::ON );
}

////////////////////////////////////////////////////////////////////////////////

HUD::~HUD() {}

////////////////////////////////////////////////////////////////////////////////

void HUD::update()
{
    if ( Data::get()->hud.enabled )
    {
        m_switch->setAllChildrenOn();

        m_sideslipAngleFPM_deg = osg::RadiansToDegrees( Data::get()->hud.sideslipAngle );
        m_angleOfAttackFPM_deg = osg::RadiansToDegrees( Data::get()->hud.angleOfAttack );

        m_validFPM = true;

        if ( fabs( m_sideslipAngleFPM_deg ) > 8.0f )
        {
            m_validFPM = false;
            m_sideslipAngleFPM_deg = ( m_sideslipAngleFPM_deg < 0.0f ) ? -8.0f : 8.0f;
        }

        if ( m_angleOfAttackFPM_deg > 12.0 || m_angleOfAttackFPM_deg < -8.0f )
        {
            m_validFPM = false;
            m_angleOfAttackFPM_deg = ( m_angleOfAttackFPM_deg < 0.0f ) ? -8.0f : 12.0f;
        }

        if ( Data::get()->hud.airspeed < 0.5f )
        {
            m_validFPM = false;
            m_angleOfAttackFPM_deg = osg::RadiansToDegrees( Data::get()->hud.pitch );
            m_sideslipAngleFPM_deg = 0.0f;
        }

        updateRollIndicator();
        updatePitchLadder();
        updateHeadingScale();

        updateFPM();
        updateILS();

        updateStall();

        updateTextL();
        updateTextR();
    }
    else
    {
        m_switch->setAllChildrenOff();
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::createWaterLine()
{
    const float w = 20.0f;
    const float h =  3.0f;

    const float d = w / 6.0f;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    m_switch->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

    v->push_back( osg::Vec3( -d * 3.0f,  0.0f, -1.0f ) ); // 1
    v->push_back( osg::Vec3( -d * 2.0f,  0.0f, -1.0f ) ); // 2
    v->push_back( osg::Vec3( -d * 1.0f,    -h, -1.0f ) ); // 3
    v->push_back( osg::Vec3(      0.0f,  0.0f, -1.0f ) ); // 4
    v->push_back( osg::Vec3(  d * 1.0f,    -h, -1.0f ) ); // 5
    v->push_back( osg::Vec3(  d * 2.0f,  0.0f, -1.0f ) ); // 6
    v->push_back( osg::Vec3(  d * 3.0f,  0.0f, -1.0f ) ); // 7

    n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

    c->push_back( osg::Vec4( Color::hud, 1.0f ) );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, 0, v->size() ) );

    geometry->setNormalArray( n.get() );
    geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );
}

////////////////////////////////////////////////////////////////////////////////

void HUD::createRollIndicator()
{
    const float r0 = 80.0f;
    const float r1 = r0 + 2.5f;
    const float r2 = r0 + 5.0f;

    osg::ref_ptr<osg::PositionAttitudeTransform> patTemp = new osg::PositionAttitudeTransform();
    m_switch->addChild( patTemp.get() );

    patTemp->setPosition( osg::Vec3( 0.0f, 0.0f, 0.0f ) );

    m_switchRollIndicator = new osg::Switch();
    patTemp->addChild( m_switchRollIndicator.get() );

    // Scale
    {
        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        patTemp->addChild( geode.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
        osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

        float a = 0.0;

        float x0 = 0.0f;
        float x1 = 0.0f;
        float x2 = 0.0f;

        float y0 = 0.0f;
        float y1 = 0.0f;
        float y2 = 0.0f;

        // -60
        a = osg::DegreesToRadians( -60.0f );

        x0 = r0 * sin( a );
        x1 = r1 * sin( a );

        y0 = r0 * cos( a );
        y1 = r1 * cos( a );

        v->push_back( osg::Vec3( -x0, y0, -1.0f ) );
        v->push_back( osg::Vec3( -x1, y1, -1.0f ) );

        v->push_back( osg::Vec3(  x0, y0, -1.0f ) );
        v->push_back( osg::Vec3(  x1, y1, -1.0f ) );

        // -45
        a = osg::DegreesToRadians( -45.0f );

        x0 = r0 * sin( a );
        x1 = r1 * sin( a );

        y0 = r0 * cos( a );
        y1 = r1 * cos( a );

        v->push_back( osg::Vec3( -x0, y0, -1.0f ) );
        v->push_back( osg::Vec3( -x1, y1, -1.0f ) );

        v->push_back( osg::Vec3(  x0, y0, -1.0f ) );
        v->push_back( osg::Vec3(  x1, y1, -1.0f ) );

        // -30
        a = osg::DegreesToRadians( -30.0f );

        x0 = r0 * sin( a );
        x2 = r2 * sin( a );

        y0 = r0 * cos( a );
        y2 = r2 * cos( a );

        v->push_back( osg::Vec3( -x0, y0, -1.0f ) );
        v->push_back( osg::Vec3( -x2, y2, -1.0f ) );

        v->push_back( osg::Vec3(  x0, y0, -1.0f ) );
        v->push_back( osg::Vec3(  x2, y2, -1.0f ) );

        // -20
        a = osg::DegreesToRadians( -20.0f );

        x0 = r0 * sin( a );
        x1 = r1 * sin( a );

        y0 = r0 * cos( a );
        y1 = r1 * cos( a );

        v->push_back( osg::Vec3( -x0, y0, -1.0f ) );
        v->push_back( osg::Vec3( -x1, y1, -1.0f ) );

        v->push_back( osg::Vec3(  x0, y0, -1.0f ) );
        v->push_back( osg::Vec3(  x1, y1, -1.0f ) );

        // -10
        a = osg::DegreesToRadians( -10.0f );

        x0 = r0 * sin( a );
        x1 = r1 * sin( a );

        y0 = r0 * cos( a );
        y1 = r1 * cos( a );

        v->push_back( osg::Vec3( -x0, y0, -1.0f ) );
        v->push_back( osg::Vec3( -x1, y1, -1.0f ) );

        v->push_back( osg::Vec3(  x0, y0, -1.0f ) );
        v->push_back( osg::Vec3(  x1, y1, -1.0f ) );

        // 0
        v->push_back( osg::Vec3(  0.0f, r0, -1.0f ) );
        v->push_back( osg::Vec3( -2.0f, r0 + 5.0f, -1.0f ) );
        v->push_back( osg::Vec3( -2.0f, r0 + 5.0f, -1.0f ) );
        v->push_back( osg::Vec3(  2.0f, r0 + 5.0f, -1.0f ) );
        v->push_back( osg::Vec3(  2.0f, r0 + 5.0f, -1.0f ) );
        v->push_back( osg::Vec3(  0.0f, r0, -1.0f ) );

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        c->push_back( osg::Vec4( Color::hud, 1.0f ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geometry->setColorArray( c.get() );
        geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }

    // Marker
    {
        m_patRollIndicator = new osg::PositionAttitudeTransform();
        m_switchRollIndicator->addChild( m_patRollIndicator.get() );

        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        m_patRollIndicator->addChild( geode.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
        osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

        v->push_back( osg::Vec3(  0.0f, r0, -1.0f ) );
        v->push_back( osg::Vec3( -2.0f, r0 - 5.0f, -1.0f ) );
        v->push_back( osg::Vec3(  2.0f, r0 - 5.0f, -1.0f ) );

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        c->push_back( osg::Vec4( Color::hud, 1.0f ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_LOOP, 0, v->size() ) );

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geometry->setColorArray( c.get() );
        geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::createPitchLadder()
{
    m_patPitchLadderRoll = new osg::PositionAttitudeTransform();
    m_switch->addChild( m_patPitchLadderRoll.get() );

    m_patPitchLadderPitch = new osg::PositionAttitudeTransform();
    m_patPitchLadderRoll->addChild( m_patPitchLadderPitch.get() );

    m_patPitchLadderWind = new osg::PositionAttitudeTransform();
    m_patPitchLadderPitch->addChild( m_patPitchLadderWind.get() );

    m_switchPitchLadder = new osg::Switch();
    m_patPitchLadderWind->addChild( m_switchPitchLadder.get() );

    const float px_deg = m_angleCoef;

    for ( int i = -90; i < 0; i++ )
    {
        if ( i % 5 == 0 )
        {
            osg::ref_ptr<osg::Geode> geode = new osg::Geode();
            m_switchPitchLadder->addChild( geode.get() );

            createPitchLadderBar( geode.get(), ((float)i)*px_deg, i );
        }
    }

    // Hozrizon Line
    {
        const float w = 120.0f;
        const float g =  20.0f;

        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        m_switchPitchLadder->addChild( geode.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
        osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

        v->push_back( osg::Vec3( -w / 2.0f, 0.0f, -1.0f ) );
        v->push_back( osg::Vec3( -g / 2.0f, 0.0f, -1.0f ) );

        v->push_back( osg::Vec3(  w / 2.0f, 0.0f, -1.0f ) );
        v->push_back( osg::Vec3(  g / 2.0f, 0.0f, -1.0f ) );

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        c->push_back( osg::Vec4( Color::hud, 1.0f ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geometry->setColorArray( c.get() );
        geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }

    for ( int i = 1; i <= 90; i++ )
    {
        if ( i % 5 == 0 )
        {
            osg::ref_ptr<osg::Geode> geode = new osg::Geode();
            m_switchPitchLadder->addChild( geode.get() );

            createPitchLadderBar( geode.get(), ((float)i)*px_deg, i );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::createHeadingScale()
{
    const float y_del = -80.0f;

    const float scale_w = 100.0f;

    const float box_w = 12.0f;
    const float box_h =  7.5f;

    const float px_deg = scale_w / 50.0;

    m_patHeadingScale = new osg::PositionAttitudeTransform();
    m_switch->addChild( m_patHeadingScale.get() );

    m_switchHeadingScale = new osg::Switch();
    m_patHeadingScale->addChild( m_switchHeadingScale.get() );

    // Box
    {
        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        m_switch->addChild( geode.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
        osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

        // marker
        v->push_back( osg::Vec3( -box_w / 6.0f, y_del + box_h * 0.5f, -1.0f ) );
        v->push_back( osg::Vec3(          0.0f, y_del               , -1.0f ) );
        v->push_back( osg::Vec3(  box_w / 6.0f, y_del + box_h * 0.5f, -1.0f ) );

        // box
        v->push_back( osg::Vec3(  box_w / 2.0f, y_del + box_h * 0.5f, -1.0f ) );
        v->push_back( osg::Vec3(  box_w / 2.0f, y_del + box_h * 1.5f, -1.0f ) );
        v->push_back( osg::Vec3( -box_w / 2.0f, y_del + box_h * 1.5f, -1.0f ) );
        v->push_back( osg::Vec3( -box_w / 2.0f, y_del + box_h * 0.5f, -1.0f ) );

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        c->push_back( osg::Vec4( Color::hud, 1.0f ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_LOOP, 0, v->size() ) );

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geometry->setColorArray( c.get() );
        geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }

    // Text Heading
    {
        m_heading = new osg::Geode();
        m_switch->addChild( m_heading.get() );

        osg::ref_ptr<osgText::Text> text = new osgText::Text();
        //text->setFont(font);
        text->setColor( osg::Vec4( Color::hud, 1.0f ) );
        text->setCharacterSize( m_charSize );
        text->setAxisAlignment( osgText::TextBase::XY_PLANE );
        text->setPosition( osg::Vec3( 0.0f, y_del + box_h * 0.5f + 1.5f, -1.0f ) );
        text->setLayout( osgText::Text::LEFT_TO_RIGHT );
        text->setAlignment( osgText::Text::CENTER_BASE_LINE );

        text->setText( "360" );
        m_heading->addDrawable( text );
    }

    // Scale
    {
        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        m_switch->addChild( geode.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
        osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

        // marker
        v->push_back( osg::Vec3( -scale_w / 2.0f, y_del, -1.0f ) );
        v->push_back( osg::Vec3(  scale_w / 2.0f, y_del, -1.0f ) );

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        c->push_back( osg::Vec4( Color::hud, 1.0f ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geometry->setColorArray( c.get() );
        geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );

        for ( int i = -360; i <= 360; i++ )
        {
            if ( i % 5 == 0 )
            {
                osg::ref_ptr<osg::Geode> geode = new osg::Geode();
                m_switchHeadingScale->addChild( geode.get() );

                if ( i % 10 == 0 )
                {
                    int deg10 = ( i < 0 ) ? ( 360 + i ) / 10 : i / 10;

                    createHeadingScaleBar( geode.get(), y_del, ((float)i)*px_deg, deg10 );
                }
                else
                {
                    createHeadingScaleBar( geode.get(), y_del, ((float)i)*px_deg );
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::createFPM()
{
    const float r0 = 3.0f;
    const float r1 = 6.0f;

    m_patFPM = new osg::PositionAttitudeTransform();
    m_switch->addChild( m_patFPM.get() );

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    m_patFPM->addChild( geode.get() );

    // Lines
    {
        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
        osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

        v->push_back( osg::Vec3( -r1, 0.0f, -1.0f ) );
        v->push_back( osg::Vec3( -r0, 0.0f, -1.0f ) );

        v->push_back( osg::Vec3(  0.0f, r1, -1.0f ) );
        v->push_back( osg::Vec3(  0.0f, r0, -1.0f ) );

        v->push_back( osg::Vec3(  r1, 0.0f, -1.0f ) );
        v->push_back( osg::Vec3(  r0, 0.0f, -1.0f ) );

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        c->push_back( osg::Vec4( Color::hud, 1.0f ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geometry->setColorArray( c.get() );
        geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }

    // Circle
    {
        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
        osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

        for ( int i = 0; i < 360; i = i + 10 )
        {
            float a = osg::DegreesToRadians( (float)i );

            float x = r0 * sin( a );
            float y = r0 * cos( a );

            v->push_back( osg::Vec3( x, y, -1.0f ) );
        }

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        c->push_back( osg::Vec4( Color::hud, 1.0f ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_LOOP, 0, v->size() ) );

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geometry->setColorArray( c.get() );
        geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }

    // invalid mark
    {
        m_switchFPM = new osg::Switch();
        m_patFPM->addChild( m_switchFPM.get() );

        osg::ref_ptr<osg::Geode> geodeCross = new osg::Geode();
        m_switchFPM->addChild( geodeCross.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
        osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

        v->push_back( osg::Vec3( -r0,  r0, -1.0f ) );
        v->push_back( osg::Vec3(  r0, -r0, -1.0f ) );

        v->push_back( osg::Vec3(  r0,  r0, -1.0f ) );
        v->push_back( osg::Vec3( -r0, -r0, -1.0f ) );

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        c->push_back( osg::Vec4( Color::hud, 1.0f ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geometry->setColorArray( c.get() );
        geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

        geodeCross->addDrawable( geometry.get() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::createILS()
{
    const float bar_w = 40.0f;
    const float mrk_w =  5.0f;

    m_patILS = new osg::PositionAttitudeTransform();
    m_switch->addChild( m_patILS.get() );

    m_switchILS = new osg::Switch();
    m_patILS->addChild( m_switchILS.get() );

    // glide slope
    {
        m_patILS_GS = new osg::PositionAttitudeTransform();
        m_switchILS->addChild( m_patILS_GS.get() );

        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        m_patILS_GS->addChild( geode.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
        osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

        // bar
        v->push_back( osg::Vec3( -bar_w/2.0f, 0.0f, -1.0f ) );
        v->push_back( osg::Vec3(  bar_w/2.0f, 0.0f, -1.0f ) );

        // marking full left
        v->push_back( osg::Vec3( -bar_w/2.0f, -mrk_w/2.0f, -1.0f ) );
        v->push_back( osg::Vec3( -bar_w/2.0f,  mrk_w/2.0f, -1.0f ) );

        // marking half left
        v->push_back( osg::Vec3( -bar_w/4.0f, -mrk_w/2.0f, -1.0f ) );
        v->push_back( osg::Vec3( -bar_w/4.0f,  mrk_w/2.0f, -1.0f ) );

        // marking center
        v->push_back( osg::Vec3(        0.0f, -mrk_w/2.0f, -1.0f ) );
        v->push_back( osg::Vec3(        0.0f,  mrk_w/2.0f, -1.0f ) );

        // marking half right
        v->push_back( osg::Vec3(  bar_w/4.0f, -mrk_w/2.0f, -1.0f ) );
        v->push_back( osg::Vec3(  bar_w/4.0f,  mrk_w/2.0f, -1.0f ) );

        // marking full right
        v->push_back( osg::Vec3(  bar_w/2.0f, -mrk_w/2.0f, -1.0f ) );
        v->push_back( osg::Vec3(  bar_w/2.0f,  mrk_w/2.0f, -1.0f ) );

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        c->push_back( osg::Vec4( Color::hud, 1.0f ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geometry->setColorArray( c.get() );
        geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }

    // localizer
    {
        m_patILS_LC = new osg::PositionAttitudeTransform();
        m_switchILS->addChild( m_patILS_LC.get() );

        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        m_patILS_LC->addChild( geode.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
        osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

        // bar
        v->push_back( osg::Vec3( 0.0f, -bar_w/2.0f, -1.0f ) );
        v->push_back( osg::Vec3( 0.0f,  bar_w/2.0f, -1.0f ) );

        // marking full bottom
        v->push_back( osg::Vec3( -mrk_w/2.0f, -bar_w/2.0f, -1.0f ) );
        v->push_back( osg::Vec3(  mrk_w/2.0f, -bar_w/2.0f, -1.0f ) );

        // marking half bottom
        v->push_back( osg::Vec3( -mrk_w/2.0f, -bar_w/4.0f, -1.0f ) );
        v->push_back( osg::Vec3(  mrk_w/2.0f, -bar_w/4.0f, -1.0f ) );

        // marking center
        v->push_back( osg::Vec3( -mrk_w/2.0f,        0.0f, -1.0f ) );
        v->push_back( osg::Vec3(  mrk_w/2.0f,        0.0f, -1.0f ) );

        // marking half top
        v->push_back( osg::Vec3( -mrk_w/2.0f,  bar_w/4.0f, -1.0f ) );
        v->push_back( osg::Vec3(  mrk_w/2.0f,  bar_w/4.0f, -1.0f ) );

        // marking full top
        v->push_back( osg::Vec3( -mrk_w/2.0f,  bar_w/2.0f, -1.0f ) );
        v->push_back( osg::Vec3(  mrk_w/2.0f,  bar_w/2.0f, -1.0f ) );

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        c->push_back( osg::Vec4( Color::hud, 1.0f ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geometry->setColorArray( c.get() );
        geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::createStall()
{
    m_switchStall = new osg::Switch();
    m_switch->addChild( m_switchStall.get() );

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    m_switchStall->addChild( geode.get() );

    osg::ref_ptr<osgText::Text> text = new osgText::Text();
    //text->setFont(font);
    text->setColor( osg::Vec4( Color::hud, 1.0f ) );
    text->setCharacterSize( 2 * m_charSize );
    text->setAxisAlignment( osgText::TextBase::XY_PLANE );
    text->setPosition( osg::Vec3( 0.0f, 20.0f, -1.0f ) );
    text->setLayout( osgText::Text::LEFT_TO_RIGHT );
    text->setAlignment( osgText::Text::CENTER_BASE_LINE );

    text->setText( "STALL" );

    geode->addDrawable( text );
}

////////////////////////////////////////////////////////////////////////////////

void HUD::createTextL()
{
    const float x = 60.0f;
    const float y =  0.0f;

    const float w = 16.0f;
    const float h =  9.0f;

    // Box
    {
        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        m_switch->addChild( geode.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
        osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors
        v->push_back( osg::Vec3( -x     , y     , -1.0f ) );
        v->push_back( osg::Vec3( -x     , y + h , -1.0f ) );
        v->push_back( osg::Vec3( -x - w , y + h , -1.0f ) );
        v->push_back( osg::Vec3( -x - w , y     , -1.0f ) );

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        c->push_back( osg::Vec4( Color::hud, 1.0f ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_LOOP, 0, v->size() ) );

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geometry->setColorArray( c.get() );
        geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }

    //osgText::Font* font = osgText::readFontFile( m_fontPath.c_str() );

    // Text Airspeed
    {
        m_airspeed = new osg::Geode();
        m_switch->addChild( m_airspeed.get() );

        osg::ref_ptr<osgText::Text> text = new osgText::Text();
        //text->setFont(font);
        text->setColor( osg::Vec4( Color::hud, 1.0f ) );
        text->setCharacterSize( m_charSize );
        text->setAxisAlignment( osgText::TextBase::XY_PLANE );
        text->setPosition( osg::Vec3( -x - 1.0f, y + 2.5f, -1.0f ) );
        text->setLayout( osgText::Text::LEFT_TO_RIGHT );
        text->setAlignment( osgText::Text::RIGHT_BASE_LINE );

        text->setText( "9999" );

        m_airspeed->addDrawable( text );
    }

    // Text Mach No
    {
        m_machNo = new osg::Geode();
        m_switch->addChild( m_machNo.get() );

        osg::ref_ptr<osgText::Text> text = new osgText::Text();
        //text->setFont(font);
        text->setColor( osg::Vec4( Color::hud, 1.0f ) );
        text->setCharacterSize( m_charSize );
        text->setAxisAlignment( osgText::TextBase::XY_PLANE );
        text->setPosition( osg::Vec3( -x - 1.0f, y - 7.5f, -1.0f ) );
        text->setLayout( osgText::Text::LEFT_TO_RIGHT );
        text->setAlignment( osgText::Text::RIGHT_BASE_LINE );

        text->setText( "2.15" );
        m_machNo->addDrawable( text );
    }

    // Text G-Force
    {
        m_gForce = new osg::Geode();
        m_switch->addChild( m_gForce.get() );

        osg::ref_ptr<osgText::Text> text = new osgText::Text();
        //text->setFont(font);
        text->setColor( osg::Vec4( Color::hud, 1.0f ) );
        text->setCharacterSize( m_charSize );
        text->setAxisAlignment( osgText::TextBase::XY_PLANE );
        text->setPosition( osg::Vec3( -x - 1.0f, y - 15.0f, -1.0f ) );
        text->setLayout( osgText::Text::LEFT_TO_RIGHT );
        text->setAlignment( osgText::Text::RIGHT_BASE_LINE );

        text->setText( "1.0G" );
        m_gForce->addDrawable( text );
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::createTextR()
{
    const float x = 60.0f;
    const float y =  0.0f;

    const float w = 23.0f;
    const float h =  9.0f;

    // Box
    {
        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        m_switch->addChild( geode.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
        osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

        v->push_back( osg::Vec3( x     , y     , -1.0f ) );
        v->push_back( osg::Vec3( x     , y + h , -1.0f ) );
        v->push_back( osg::Vec3( x + w , y + h , -1.0f ) );
        v->push_back( osg::Vec3( x + w , y     , -1.0f ) );

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        c->push_back( osg::Vec4( Color::hud, 1.0f ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_LOOP, 0, v->size() ) );

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geometry->setColorArray( c.get() );
        geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }

    //osgText::Font* font = osgText::readFontFile( m_fontPath.c_str() );

    // Text Altitude
    {
        m_altitude = new osg::Geode();
        m_switch->addChild( m_altitude.get() );

        osg::ref_ptr<osgText::Text> text = new osgText::Text();
        //text->setFont(font);
        text->setColor( osg::Vec4( Color::hud, 1.0f ) );
        text->setCharacterSize( m_charSize );
        text->setAxisAlignment( osgText::TextBase::XY_PLANE );
        text->setPosition( osg::Vec3( x + w - 1.0f, y + 2.5f, -1.0f ) );
        text->setLayout( osgText::Text::LEFT_TO_RIGHT );
        text->setAlignment( osgText::Text::RIGHT_BASE_LINE );

        text->setText( "99,999" );
        m_altitude->addDrawable( text );
    }

    // Text Climb Rate
    {
        m_climbRate = new osg::Geode();
        m_switch->addChild( m_climbRate.get() );

        osg::ref_ptr<osgText::Text> text = new osgText::Text();
        //text->setFont(font);
        text->setColor( osg::Vec4( Color::hud, 1.0f ) );
        text->setCharacterSize( m_charSize );
        text->setAxisAlignment( osgText::TextBase::XY_PLANE );
        text->setPosition( osg::Vec3( x + w - 1.0f, y - 7.5f, -1.0f ) );
        text->setLayout( osgText::Text::LEFT_TO_RIGHT );
        text->setAlignment( osgText::Text::RIGHT_BASE_LINE );

        text->setText( "6000" );
        m_climbRate->addDrawable( text );
    }

    // Text Radio Altitude
    {
        m_radioAlt = new osg::Geode();
        m_switch->addChild( m_radioAlt.get() );

        osg::ref_ptr<osgText::Text> text = new osgText::Text();
        //text->setFont(font);
        text->setColor( osg::Vec4( Color::hud, 1.0f ) );
        text->setCharacterSize( m_charSize );
        text->setAxisAlignment( osgText::TextBase::XY_PLANE );
        text->setPosition( osg::Vec3( x + w - 1.0f, y - 15.0f, -1.0f ) );
        text->setLayout( osgText::Text::LEFT_TO_RIGHT );
        text->setAlignment( osgText::Text::RIGHT_BASE_LINE );

        text->setText( "RA 9999" );
        m_radioAlt->addDrawable( text );
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::createPitchLadderBar( osg::Geode *geode, int y, int deg )
{
    const float w = 60.0f; // width
    const float h =  3.0f; // height
    const float g = 20.0f; // gap

    // Bar
    {
        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
        osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

        if ( deg < 0 )
        {
            float l = w - g;
            float d = l * 0.2f;

            // left
            v->push_back( osg::Vec3( -w / 2.0f    , y, -1.0f ) );
            v->push_back( osg::Vec3( -w / 2.0f + d, y, -1.0f ) );

            v->push_back( osg::Vec3( -g / 2.0f - d, y, -1.0f ) );
            v->push_back( osg::Vec3( -g / 2.0f    , y, -1.0f ) );

            v->push_back( osg::Vec3( -g / 2.0f,     y, -1.0f ) );
            v->push_back( osg::Vec3( -g / 2.0f, y + h, -1.0f ) );

            // right
            v->push_back( osg::Vec3(  w / 2.0f    , y, -1.0f ) );
            v->push_back( osg::Vec3(  w / 2.0f - d, y, -1.0f ) );

            v->push_back( osg::Vec3(  g / 2.0f + d, y, -1.0f ) );
            v->push_back( osg::Vec3(  g / 2.0f    , y, -1.0f ) );

            v->push_back( osg::Vec3(  g / 2.0f,     y, -1.0f ) );
            v->push_back( osg::Vec3(  g / 2.0f, y + h, -1.0f ) );
        }
        else
        {
            // left
            v->push_back( osg::Vec3( -w / 2.0f,     y, -1.0f ) );
            v->push_back( osg::Vec3( -g / 2.0f,     y, -1.0f ) );

            v->push_back( osg::Vec3( -g / 2.0f,     y, -1.0f ) );
            v->push_back( osg::Vec3( -g / 2.0f, y - h, -1.0f ) );

            // right
            v->push_back( osg::Vec3(  w / 2.0f,     y, -1.0f ) );
            v->push_back( osg::Vec3(  g / 2.0f,     y, -1.0f ) );

            v->push_back( osg::Vec3(  g / 2.0f,     y, -1.0f ) );
            v->push_back( osg::Vec3(  g / 2.0f, y - h, -1.0f ) );
        }

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        c->push_back( osg::Vec4( Color::hud, 1.0f ) );

        geometry->setVertexArray( v.get() );

        if ( deg < 0 )
        {
            geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );
        }
        else
        {
            geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );
        }

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geometry->setColorArray( c.get() );
        geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }

    char deg_srt[256];

    if ( deg < 0 )
    {
        sprintf( deg_srt, "%d", -deg );
    }
    else
    {
        sprintf( deg_srt, "%d",  deg );
    }

    // TEXT
    float y_del = m_charSize - 3.0;

    if ( deg < 0 )
    {
        y_del = -m_charSize - 0.5;
    }

    //osgText::Font* font = osgText::readFontFile( m_fontPath.c_str() );

    // text L
    {
        osg::ref_ptr<osgText::Text> text = new osgText::Text();
        //text->setFont(font);
        text->setColor( osg::Vec4( Color::hud, 1.0f ) );
        text->setCharacterSize( m_charSize );
        text->setAxisAlignment( osgText::TextBase::XY_PLANE );
        text->setPosition( osg::Vec3( -w / 2.0f, y + y_del, -1.0f ) );
        text->setLayout( osgText::Text::LEFT_TO_RIGHT );
        text->setAlignment( osgText::Text::LEFT_BASE_LINE );

        text->setText( deg_srt );
        geode->addDrawable(text);
    }

    // text R
    {
        osg::ref_ptr<osgText::Text> text = new osgText::Text();
        //text->setFont(font);
        text->setColor( osg::Vec4( Color::hud, 1.0f ) );
        text->setCharacterSize( m_charSize );
        text->setAxisAlignment( osgText::TextBase::XY_PLANE );
        text->setPosition( osg::Vec3( w / 2.0f, y + y_del, -1.0f ) );
        text->setLayout( osgText::Text::LEFT_TO_RIGHT );
        text->setAlignment( osgText::Text::RIGHT_BASE_LINE );

        text->setText( deg_srt );
        geode->addDrawable(text);
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::createHeadingScaleBar( osg::Geode *geode, float y_del, int x, int deg10 )
{
    /////////////////////////////////////////
    createHeadingScaleBar( geode, y_del, x );
    /////////////////////////////////////////

    char deg10_srt[256];

    sprintf( deg10_srt, "%02d", deg10 );

    osg::ref_ptr<osgText::Text> text = new osgText::Text();
    //text->setFont(font);
    text->setColor( osg::Vec4( Color::hud, 1.0f ) );
    text->setCharacterSize( m_charSize );
    text->setAxisAlignment( osgText::TextBase::XY_PLANE );
    text->setPosition( osg::Vec3( x, y_del - m_charSize - 3.0f, -1.0f ) );
    text->setLayout( osgText::Text::LEFT_TO_RIGHT );
    text->setAlignment( osgText::Text::CENTER_BASE_LINE );

    text->setText( deg10_srt );

    geode->addDrawable(text);
}

////////////////////////////////////////////////////////////////////////////////

void HUD::createHeadingScaleBar( osg::Geode *geode, float y_del, int x )
{
    const float h = 2.5f;

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

    // bar
    v->push_back( osg::Vec3( x, y_del    , -1.0f ) );
    v->push_back( osg::Vec3( x, y_del - h, -1.0f ) );

    n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

    c->push_back( osg::Vec4( Color::hud, 1.0f ) );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );

    geometry->setNormalArray( n.get() );
    geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );
}

////////////////////////////////////////////////////////////////////////////////

void HUD::updateRollIndicator()
{
    if ( fabs( Data::get()->hud.roll ) < m_rollLimit )
    {
        m_switchRollIndicator->setAllChildrenOn();

        osg::Quat q( Data::get()->hud.roll, osg::Z_AXIS );

        m_patRollIndicator->setAttitude( q );
    }
    else
    {
        m_switchRollIndicator->setAllChildrenOff();
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::updatePitchLadder()
{
    float roll_deg  = osg::RadiansToDegrees( Data::get()->hud.roll  );
    float pitch_deg = osg::RadiansToDegrees( Data::get()->hud.pitch );

    osg::Quat q( Data::get()->hud.roll, osg::Z_AXIS );

    m_patPitchLadderRoll->setAttitude( q );

    osg::Vec3 r( 0.0, -m_angleCoef * pitch_deg, 0.0 );

    m_patPitchLadderPitch->setPosition( r );

    float sinRoll = sin( Data::get()->hud.roll );
    float cosRoll = cos( Data::get()->hud.roll );

    float x = -m_angleCoef * ( -m_sideslipAngleFPM_deg * cosRoll + m_angleOfAttackFPM_deg * sinRoll );

    m_patPitchLadderWind->setPosition( osg::Vec3( x, 0.0, 0.0 ) );

    short i_pitch = floor( ( 90.0 + pitch_deg ) / 5.0 + 0.5 );

    if ( i_pitch <  0 ) i_pitch =  0;
    if ( i_pitch > 36 ) i_pitch = 36;

    float pitch_i_deg = i_pitch * 5.0f - 90.0f;

    m_switchPitchLadder->setAllChildrenOff();
    m_switchPitchLadder->setValue( i_pitch, true );

    if ( i_pitch >  0 ) m_switchPitchLadder->setValue( i_pitch - 1, true );
    if ( i_pitch < 36 ) m_switchPitchLadder->setValue( i_pitch + 1, true );

    float roll_deg_abs = fabs( roll_deg );

    if ( roll_deg_abs < 30.0 || roll_deg_abs > 150.0 )
    {
        if ( i_pitch >  1 && pitch_deg - pitch_i_deg <  1.0f )
        {
            m_switchPitchLadder->setValue( i_pitch - 2, true );
        }

        if ( i_pitch < 35 && pitch_deg - pitch_i_deg > -1.0f )
        {
            m_switchPitchLadder->setValue( i_pitch + 2, true );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::updateHeadingScale()
{
    osg::ref_ptr<osgText::Text> text;

    float head_deg = osg::RadiansToDegrees( Data::get()->hud.heading );

    short head_ind = floor( ( ( ( head_deg < 180.0f ) ? 360.0f : 0.0f ) + head_deg ) / 5.0 + 0.5 );
    if ( head_ind <   0 ) head_ind =   0;
    if ( head_ind > 128 ) head_ind = 128;

    m_switchHeadingScale->setSingleChildOn( head_ind );
    m_switchHeadingScale->setValue( head_ind - 1, true );
    m_switchHeadingScale->setValue( head_ind + 1, true );
    m_switchHeadingScale->setValue( head_ind - 2, true );
    m_switchHeadingScale->setValue( head_ind + 2, true );
    m_switchHeadingScale->setValue( head_ind - 3, true );
    m_switchHeadingScale->setValue( head_ind + 3, true );
    m_switchHeadingScale->setValue( head_ind - 4, true );
    m_switchHeadingScale->setValue( head_ind + 4, true );

    float x = ( 100.0f / 50.0f ) * ( ( head_deg < 180.0f ) ? head_deg : head_deg - 360.0f );
    m_patHeadingScale->setPosition( osg::Vec3( -x, 0.0, 0.0 ) );

    text = dynamic_cast< osgText::Text* >( m_heading->getDrawable( 0 ) );

    if ( text.valid() )
    {
        char heading_str[256];
        sprintf( heading_str, "%d", (int)( floor( head_deg + 0.5 ) ) );
        text->setText( heading_str );
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::updateFPM()
{
    if ( m_validFPM )
    {
        m_switchFPM->setAllChildrenOff();
    }
    else
    {
        m_switchFPM->setAllChildrenOn();
    }

    osg::Vec3 r(  m_angleCoef * m_sideslipAngleFPM_deg,
                 -m_angleCoef * m_angleOfAttackFPM_deg,
                  0.0 );

    m_patFPM->setPosition( r );
}

////////////////////////////////////////////////////////////////////////////////

void HUD::updateILS()
{
    if ( Data::get()->hud.ils_visible )
    {
        m_switchILS->setAllChildrenOn();

        osg::Quat q( Data::get()->hud.roll, osg::Z_AXIS );
        osg::Vec3 r(  m_angleCoef * m_sideslipAngleFPM_deg,
                     -m_angleCoef * m_angleOfAttackFPM_deg,
                      0.0 );

        m_patILS->setAttitude( q );
        m_patILS->setPosition( r );

        float ils_gs = Data::get()->hud.ils_gs_deviation;
        float ils_lc = Data::get()->hud.ils_lc_deviation;

        if ( ils_gs < -1.0f ) ils_gs = -1.0f;
        if ( ils_gs >  1.0f ) ils_gs =  1.0f;
        if ( ils_lc < -1.0f ) ils_lc = -1.0f;
        if ( ils_lc >  1.0f ) ils_lc =  1.0f;

        m_patILS_GS->setPosition( osg::Vec3( 0.0, 20.0 * ils_gs, 0.0 ) );
        m_patILS_LC->setPosition( osg::Vec3( 20.0 * ils_lc, 0.0, 0.0 ) );
    }
    else
    {
        m_switchILS->setAllChildrenOff();
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::updateStall()
{
    if ( Data::get()->hud.stall )
    {
        m_switchStall->setAllChildrenOn();
    }
    else
    {
        m_switchStall->setAllChildrenOff();
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::updateTextL()
{
    osg::ref_ptr<osgText::Text> text;

    // airspeed
    text = dynamic_cast< osgText::Text* >( m_airspeed->getDrawable( 0 ) );

    if ( text.valid() )
    {
        float airspeed_kt = fdm::Units::mps2kts( Data::get()->hud.airspeed );

        char airspeed_str[256];
        sprintf( airspeed_str, "%d", (int)( floor( airspeed_kt + 0.5 ) ) );
        text->setText( airspeed_str );
    }

    // Mach no
    text = dynamic_cast< osgText::Text* >( m_machNo->getDrawable( 0 ) );

    if ( text.valid() )
    {
        char machNo_str[256];
        sprintf( machNo_str, "M %.2f", Data::get()->hud.machNumber );
        text->setText( machNo_str );
    }

    // G-Force
    text = dynamic_cast< osgText::Text* >( m_gForce->getDrawable( 0 ) );

    if ( text.valid() )
    {
        char gForce_str[256];
        sprintf( gForce_str, "%.1fG", Data::get()->hud.g_force );
        text->setText( gForce_str );
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::updateTextR()
{
    osg::ref_ptr<osgText::Text> text;

    // altitude
    text = dynamic_cast< osgText::Text* >( m_altitude->getDrawable( 0 ) );

    if ( text.valid() )
    {
        float altitude_ft = fdm::Units::m2ft( Data::get()->hud.altitude );

        char altitude_str[256];
        sprintf( altitude_str, "%d", (int)( floor( altitude_ft + 0.5 ) ) );
        text->setText( altitude_str );
    }

    // climb rate
    text = dynamic_cast< osgText::Text* >( m_climbRate->getDrawable( 0 ) );

    if ( text.valid() )
    {
        float climbRate_fpm = fdm::Units::mps2fpm( Data::get()->hud.climbRate );

        char climbRate_str[256];
        sprintf( climbRate_str, "%d", (int)( floor( climbRate_fpm + 0.5 ) ) );
        text->setText( climbRate_str );
    }

    // radio altitude
    text = dynamic_cast< osgText::Text* >( m_radioAlt->getDrawable( 0 ) );

    if ( text.valid() )
    {
        float altitude_ft = fdm::Units::m2ft( Data::get()->hud.radioAlt );

        if ( altitude_ft < 3000.0 )
        {
            char altitude_str[256];
            sprintf( altitude_str, "RA %d", (int)( floor( altitude_ft + 0.5 ) ) );
            text->setText( altitude_str );
        }
        else
        {
            text->setText( "" );
        }
    }
}
