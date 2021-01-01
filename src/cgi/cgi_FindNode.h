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
#ifndef CGI_FINDNODE_H
#define CGI_FINDNODE_H

////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>

#include <osg/NodeVisitor>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief Finding node visitor class.
 */
class FindNode : public osg::NodeVisitor
{
public:

    typedef std::vector< osg::ref_ptr<osg::Node> > Nodes;

    /** */
    static osg::Node* findFirst( osg::Node *node, const char *name );

    /** */
    static Nodes findNodes( osg::Node *node, const char *name );

    /** @brief Constructor. */
    FindNode();

    /** @brief Constructor. */
    FindNode (const char *name, bool findAll = false );

    /** */
    virtual void apply( osg::Node &searchNode );

    /** @brief Returns first found node. */
    osg::ref_ptr<osg::Node> getFirst();

    /** @brief Returns list of found nodes. */
    Nodes getNodes();

private:

    bool _findAll;      ///< specifies if search should be continued after finding first matching node
    std::string _name;  ///< name of the nodes to be looking for
    Nodes _nodes;       ///< found nodes list
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_FINDNODE_H
