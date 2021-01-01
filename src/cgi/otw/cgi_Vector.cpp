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

#include <cgi/otw/cgi_Vector.h>

#include <Data.h>

#include <cgi/cgi_Utils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Vector::Vector()
{
    _root = new osg::Group();
}

////////////////////////////////////////////////////////////////////////////////

Vector::~Vector() {}

////////////////////////////////////////////////////////////////////////////////

void Vector::update()
{
    if ( _root->getNumChildren() > 0 )
    {
        _root->removeChildren( 0, _root->getNumChildren() );
    }

    if ( Data::get()->cgi.vector.visible )
    {
        osg::Vec3 b( Data::get()->cgi.vector.b_x,
                     Data::get()->cgi.vector.b_y,
                     Data::get()->cgi.vector.b_z );

        osg::Vec3 v( Data::get()->cgi.vector.v_x,
                     Data::get()->cgi.vector.v_y,
                     Data::get()->cgi.vector.v_z );

        Utils::drawVector( _root.get(), b, v );
    }
}
