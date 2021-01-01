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
#ifndef CGI_MODULE_H
#define CGI_MODULE_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/Group>

#include <cgi/cgi_Defines.h>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief Simulation CGI module base class.
 */
class Module
{
public:

    typedef std::vector< Module* > List;

    /**
     * @brief Constructor.
     * @param parent parent module
     */
    Module( const Module *parent = NULLPTR );

    /** @brief Destructor. */
    virtual ~Module();

    /** @brief Adds child to the module. */
    virtual void addChild( Module *child );

    /** @brief Updates module and all its children. */
    virtual void update();

    /** @brief Returns module root node.  */
    inline osg::Group* getNode() { return _root.get(); }

protected:

    const Module *_parent;              ///< parent node

    osg::ref_ptr<osg::Group> _root;     ///< OSG module root node
    List _children;                     ///< children nodes

    /** Removes all children. */
    virtual void removeAllChildren();
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_MODULE_H
