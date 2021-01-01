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
#ifndef G1000_MODULE_H
#define G1000_MODULE_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/Group>
#include <osg/Vec3>

#include <g1000/sim/g1000_IFD.h>

////////////////////////////////////////////////////////////////////////////////

namespace g1000
{

/**
 * @brief GDU module base class.
 */
class Module
{
public:

    /** Constructor. */
    Module( IFD *ifd );

    /** Destructor. */
    virtual ~Module();

    /** Updates module. */
    virtual void update() = 0;

    /** Returns module OSG root node.  */
    inline osg::Group* getNode() { return _root.get(); }

protected:

    const IFD *_ifd;                    ///< IFD object

    osg::ref_ptr<osg::Group> _root;     ///< OSG module root node
};

} // end of g1000 namespace

////////////////////////////////////////////////////////////////////////////////

#endif // G1000_MODULE_H
