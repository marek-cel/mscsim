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
#ifndef G1000_GDU_H
#define G1000_GDU_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/Group>

#include <g1000/g1000_Data.h>

////////////////////////////////////////////////////////////////////////////////

namespace g1000
{

/**
 * @brief GDU base class.
 */
class GDU
{
public:

    /** Constructor. */
    GDU();

    /** Destructor. */
    virtual ~GDU();

    /** Initializes display. */
    virtual void init( const std::string &file ) = 0;

    /** Updates display. */
    virtual void update( const Data &data ) = 0;

    inline osg::Group* getRoot() { return _root.get(); }

protected:

    osg::ref_ptr<osg::Group> _root;
};

} // end of g1000 namespace

////////////////////////////////////////////////////////////////////////////////

#endif // G1000_GDU_H
