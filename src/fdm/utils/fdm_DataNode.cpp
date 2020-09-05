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

#include <fdm/utils/fdm_DataNode.h>

#include <limits>

#include <fdm/utils/fdm_String.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

DataNode::DataNode() :
    _parent( 0 ),
    _type( Group )
{
    _children.clear();
}

////////////////////////////////////////////////////////////////////////////////

DataNode::~DataNode()
{
    if ( _type == Group )
    {
        DataNodes::iterator it;

        it = _children.begin();

        while ( it != _children.end() )
        {
            if ( it->second )
            {
                delete it->second;
                it->second = 0;
            }

            ++it;
        }
    }

    _children.clear();
}

////////////////////////////////////////////////////////////////////////////////

int DataNode::addNode( const char *path, Type type )
{
    if ( _type == Group )
    {
        std::string path_temp = path;

        path_temp = stripPathDots( path_temp.c_str() );

        if ( path_temp.length() > 0 )
        {
            path_temp = String::toLower( path_temp );

            std::string pathLead;
            std::string pathRest;

            breakPath( path_temp.c_str(), pathLead, pathRest );

            if ( pathRest.size() > 0 )
            {
                DataNode *node = 0;

                if ( _children.count( pathLead ) == 0 )
                {
                    addNode( pathLead.c_str(), Group );
                }

                node = findNode( pathLead.c_str() );

                if ( node )
                {
                    return node->addNode( pathRest.c_str(), type );
                }
            }
            else
            {
                if ( pathLead.size() > 0 )
                {
                    if ( _children.count( pathLead ) == 0 )
                    {
                        DataNode *node = createNode( pathLead.c_str(), type, this );

                        std::pair<DataNodes::iterator,bool> result;

                        result = _children.insert( std::pair<std::string,DataNode*>( pathLead, node ) );

                        if ( result.second == true )
                        {
                            return FDM_SUCCESS;
                        }
                        else
                        {
                            delete node;
                        }
                    }
                }
            }
        }
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

bool DataNode::getDatab() const
{
    if ( _type == Bool )
    {
        return _data.bData;
    }

    return std::numeric_limits< bool >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

int DataNode::getDatai() const
{
    if ( _type == Int )
    {
        return _data.iData;
    }

    return std::numeric_limits< int >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

long DataNode::getDatal() const
{
    if ( _type == Long )
    {
        return _data.lData;
    }

    return std::numeric_limits< long >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

float DataNode::getDataf() const
{
    if ( _type == Float )
    {
        return _data.fData;
    }

    return std::numeric_limits< float >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

double DataNode::getDatad() const
{
    if ( _type == Double )
    {
        return _data.dData;
    }

    return std::numeric_limits< double >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

DataNode* DataNode::getNode( const char *path )
{
    std::string path_temp = String::toLower( path );

    return findNode( path_temp.c_str() );
}

////////////////////////////////////////////////////////////////////////////////

std::string DataNode::getPath() const
{
    std::string path;
    path.clear();

    if ( _parent )
    {
        path += _parent->getPath();
    }

    path += ".";
    path += _name;

    return path;
}

////////////////////////////////////////////////////////////////////////////////

DataNode* DataNode::getRoot()
{
    if ( _parent )
    {
        return _parent->getRoot();
    }
    else
    {
        return this;
    }
}

////////////////////////////////////////////////////////////////////////////////

double DataNode::getValue() const
{
    switch ( _type )
    {
        case Group:  return std::numeric_limits< double >::quiet_NaN(); break;
        case Bool:   return (double)_data.bData; break;
        case Int:    return (double)_data.iData; break;
        case Long:   return (double)_data.lData; break;
        case Float:  return (double)_data.fData; break;
        case Double: return (double)_data.dData; break;
    }

    return std::numeric_limits< double >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

int DataNode::setDatab( bool value )
{
    if ( _type == Bool )
    {
        _data.bData = value;
        return FDM_SUCCESS;
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

int DataNode::setDatai( int value )
{
    if ( _type == Int )
    {
        _data.iData = value;
        return FDM_SUCCESS;
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

int DataNode::setDatal( long value )
{
    if ( _type == Long )
    {
        _data.lData = value;
        return FDM_SUCCESS;
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

int DataNode::setDataf( float value )
{
    if ( _type == Float )
    {
        _data.fData = value;
        return FDM_SUCCESS;
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

int DataNode::setDatad( double value )
{
    if ( _type == Double )
    {
        _data.dData = value;
        return FDM_SUCCESS;
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

int DataNode::setValue( double value )
{
    switch ( _type )
    {
        case Bool:   _data.bData = value != 0.0;  return FDM_SUCCESS; break;
        case Int:    _data.iData = (int)   value; return FDM_SUCCESS; break;
        case Long:   _data.lData = (long)  value; return FDM_SUCCESS; break;
        case Float:  _data.fData = (float) value; return FDM_SUCCESS; break;
        case Double: _data.dData =         value; return FDM_SUCCESS; break;

        default: return FDM_FAILURE; break;
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

void DataNode::breakPath( const char *path, std::string &pathLead, std::string &pathRest )
{
    std::string str( path );

    size_t pos = str.find( '.' );

    pathLead.clear();

    if ( pos != std::string::npos )
    {
        pathLead = str.substr( 0 , pos );
        pathRest = str.substr( pos + 1 );
    }
    else
    {
        pathLead = path;
        pathRest.clear();
    }
}

////////////////////////////////////////////////////////////////////////////////

DataNode* DataNode::createNode( const char *name, Type type, DataNode *parent )
{
    std::string name_temp = String::toLower( name );

    DataNode *node = new DataNode();

    node->_parent = parent;

    node->_name = name_temp;
    node->_type = type;

    switch ( node->_type )
    {
        case Group:  node->_children.clear();    break;
        case Bool:   node->_data.bData = false;  break;
        case Int:    node->_data.iData = 0;      break;
        case Long:   node->_data.lData = 0L;     break;
        case Float:  node->_data.fData = 0.0f;   break;
        case Double: node->_data.dData = 0.0;    break;
    }

    return node;
}

////////////////////////////////////////////////////////////////////////////////

DataNode* DataNode::findNode( const char *path )
{
    std::string path_temp = path;

    stripPathDots( path_temp.c_str() );

    if ( path_temp.length() > 0 )
    {
        std::string pathLead;
        std::string pathRest;

        breakPath( path_temp.c_str(), pathLead, pathRest );

        DataNodes::iterator it;

        it = _children.find( pathLead );

        if ( it != _children.end() )
        {
            if ( pathRest.size() > 0 )
            {
                return it->second->findNode( pathRest.c_str() );
            }
            else
            {
                return it->second;
            }
        }
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

std::string DataNode::stripPathDots( const char *path )
{
    std::string str( path );

    size_t pos;

    // removing leading dots
    pos = str.find( '.' );

    if ( pos != std::string::npos )
    {
        while ( pos == 0 )
        {
            str = str.substr( pos + 1 );
            pos = str.find( '.' );
        }
    }

    // removing trailing dots
    pos = str.rfind( '.' );

    if ( pos != std::string::npos )
    {
        while ( pos == str.length() - 1 )
        {
            str = str.substr( 0, pos );
            pos = str.rfind( '.' );
        }
    }

    return str;
}
