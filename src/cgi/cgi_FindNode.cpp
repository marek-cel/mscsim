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

#include <cgi/cgi_FindNode.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

osg::Node* FindNode::findFirst( osg::Node *node, const char *name )
{
    if ( node )
    {
        FindNode findNode( name );
        node->accept( findNode );
        return dynamic_cast<osg::Node*>( findNode.getFirst().get() );
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

FindNode::Nodes FindNode::findNodes( osg::Node *node, const char *name )
{
    if ( node )
    {
        FindNode findNode( name, true );
        node->accept( findNode );
        return findNode.getNodes();
    }

    return Nodes();
}

////////////////////////////////////////////////////////////////////////////////

FindNode::FindNode() :
    osg::NodeVisitor( TRAVERSE_ALL_CHILDREN ),

    _findAll ( false )
{
    _name = "";
    _nodes.clear();
}

////////////////////////////////////////////////////////////////////////////////

FindNode::FindNode( const char *name, bool findAll ) :
    osg::NodeVisitor( TRAVERSE_ALL_CHILDREN ),

    _findAll ( findAll )
{
    _name = name;
    _nodes.clear();
}

////////////////////////////////////////////////////////////////////////////////

void FindNode::apply( osg::Node &searchNode )
{
    if ( _name == searchNode.getName() || _name.length() == 0 )
    {
        osg::ref_ptr<osg::Node> tempNode = &searchNode;

        _nodes.push_back( tempNode.get() );

        if ( _findAll ) traverse( searchNode );
    }
    else
    {
        traverse( searchNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

osg::ref_ptr<osg::Node> FindNode::getFirst()
{
    if ( _nodes.empty() )
    {
        return 0;
    }
    else
    {
        return _nodes[ 0 ];
    }
}

////////////////////////////////////////////////////////////////////////////////

FindNode::Nodes FindNode::getNodes()
{
    return _nodes;
}
