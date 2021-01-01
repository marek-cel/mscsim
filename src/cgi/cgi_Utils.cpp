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

#include <cgi/cgi_Utils.h>

#include <osg/LineWidth>

#include <cgi/cgi_Colors.h>
#include <cgi/cgi_Geometry.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

void Utils::drawVector( osg::Group *parent, const osg::Vec3 &b, const osg::Vec3 &v )
{
    osg::Vec3 ev = b + v;
    osg::Vec3 ex = b + osg::Vec3( v.x(), 0.0, 0.0 );
    osg::Vec3 ey = b + osg::Vec3( 0.0, v.y(), 0.0 );
    osg::Vec3 ez = b + osg::Vec3( 0.0, 0.0, v.z() );

    osg::ref_ptr<osg::Geode> geodev = new osg::Geode();
    osg::ref_ptr<osg::Geode> geodex = new osg::Geode();
    osg::ref_ptr<osg::Geode> geodey = new osg::Geode();
    osg::ref_ptr<osg::Geode> geodez = new osg::Geode();

    parent->addChild( geodev.get() );
    parent->addChild( geodex.get() );
    parent->addChild( geodey.get() );
    parent->addChild( geodez.get() );

    osg::ref_ptr<osg::Geometry> geometryv = new osg::Geometry();
    osg::ref_ptr<osg::Geometry> geometryx = new osg::Geometry();
    osg::ref_ptr<osg::Geometry> geometryy = new osg::Geometry();
    osg::ref_ptr<osg::Geometry> geometryz = new osg::Geometry();

    geodev->addDrawable( geometryv.get() );
    geodex->addDrawable( geometryx.get() );
    geodey->addDrawable( geometryy.get() );
    geodez->addDrawable( geometryz.get() );

    Geometry::createLine( geometryv.get(), b, ev, Colors::_amber );
    Geometry::createLine( geometryx.get(), b, ex, Colors::_red   );
    Geometry::createLine( geometryy.get(), b, ey, Colors::_lime  );
    Geometry::createLine( geometryz.get(), b, ez, Colors::_blue  );

    osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth();
    lineWidth->setWidth( 2.0f );

    geodev->getOrCreateStateSet()->setAttributeAndModes( lineWidth, osg::StateAttribute::ON );
    geodex->getOrCreateStateSet()->setAttributeAndModes( lineWidth, osg::StateAttribute::ON );
    geodey->getOrCreateStateSet()->setAttributeAndModes( lineWidth, osg::StateAttribute::ON );
    geodez->getOrCreateStateSet()->setAttributeAndModes( lineWidth, osg::StateAttribute::ON );
}
