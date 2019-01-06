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

#include <fdmMain/fdm_DataNode.h>

#include <limits>

#include <fdmUtils/fdm_String.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

DataNode::DataNode() :
    m_parent( 0 ),
    m_type( Group )
{
    m_children.clear();
}

////////////////////////////////////////////////////////////////////////////////

DataNode::~DataNode()
{
    if ( m_type == Group )
    {
        DataNodes::iterator it;

        it = m_children.begin();

        while ( it != m_children.end() )
        {
            if ( it->second )
            {
                delete it->second;
                it->second = 0;
            }

            ++it;
        }
    }

    m_children.clear();
}

////////////////////////////////////////////////////////////////////////////////

int DataNode::addNode( const std::string &path, Type type )
{
    if ( m_type == Group )
    {
        std::string path_temp = path;

        stripPathSlashes( path_temp );

        if ( path_temp.length() > 0 )
        {
            path_temp = String::toLower( path_temp );

            std::string pathLead;
            std::string pathRest;

            breakPath( path_temp, pathLead, pathRest );

            if ( pathRest.size() > 0 )
            {
                DataNode *node = 0;

                if ( m_children.count( pathLead ) == 0 )
                {
                    addNode( pathLead, Group );
                }

                node = findNode( pathLead );

                if ( node )
                {
                    return node->addNode( pathRest, type );
                }
            }
            else
            {
                if ( pathLead.size() > 0 )
                {
                    if ( m_children.count( pathLead ) == 0 )
                    {
                        DataNode *node = createNode( pathLead, type, this );

                        std::pair<DataNodes::iterator,bool> result;

                        result = m_children.insert( std::pair<std::string,DataNode*>( pathLead, node ) );

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
    if ( m_type == Bool )
    {
        return m_data.bData;
    }

    return std::numeric_limits< bool >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

int DataNode::getDatai() const
{
    if ( m_type == Int )
    {
        return m_data.iData;
    }

    return std::numeric_limits< int >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

long DataNode::getDatal() const
{
    if ( m_type == Long )
    {
        return m_data.lData;
    }

    return std::numeric_limits< long >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

float DataNode::getDataf() const
{
    if ( m_type == Float )
    {
        return m_data.fData;
    }

    return std::numeric_limits< float >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

double DataNode::getDatad() const
{
    if ( m_type == Double )
    {
        return m_data.dData;
    }

    return std::numeric_limits< double >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

DataNode* DataNode::getNode( const std::string &path )
{
    std::string path_temp = String::toLower( path );

    return findNode( path_temp );
}

////////////////////////////////////////////////////////////////////////////////

std::string DataNode::getPath() const
{
    std::string path;
    path.clear();

    if ( m_parent )
    {
        path += m_parent->getPath();
    }

    path += "/";
    path += m_name;

    return path;
}

////////////////////////////////////////////////////////////////////////////////

DataNode* DataNode::getRoot()
{
    if ( m_parent )
    {
        return m_parent->getRoot();
    }
    else
    {
        return this;
    }
}

////////////////////////////////////////////////////////////////////////////////

double DataNode::getValue() const
{
    switch ( m_type )
    {
        case Group:  return std::numeric_limits< double >::quiet_NaN(); break;
        case Bool:   return (double)m_data.bData; break;
        case Int:    return (double)m_data.iData; break;
        case Long:   return (double)m_data.lData; break;
        case Float:  return (double)m_data.fData; break;
        case Double: return (double)m_data.dData; break;
    }

    return std::numeric_limits< double >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

int DataNode::setDatab( bool value )
{
    if ( m_type == Bool )
    {
        m_data.bData = value;
        return FDM_SUCCESS;
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

int DataNode::setDatai( int value )
{
    if ( m_type == Int )
    {
        m_data.iData = value;
        return FDM_SUCCESS;
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

int DataNode::setDatal( long value )
{
    if ( m_type == Long )
    {
        m_data.lData = value;
        return FDM_SUCCESS;
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

int DataNode::setDataf( float value )
{
    if ( m_type == Float )
    {
        m_data.fData = value;
        return FDM_SUCCESS;
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

int DataNode::setDatad( double value )
{
    if ( m_type == Double )
    {
        m_data.dData = value;
        return FDM_SUCCESS;
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

int DataNode::setValue( double value )
{
    switch ( m_type )
    {
        case Bool:   m_data.bData = value != 0.0;  return FDM_SUCCESS; break;
        case Int:    m_data.iData = (int)   value; return FDM_SUCCESS; break;
        case Long:   m_data.lData = (long)  value; return FDM_SUCCESS; break;
        case Float:  m_data.fData = (float) value; return FDM_SUCCESS; break;
        case Double: m_data.dData =         value; return FDM_SUCCESS; break;

        default: return FDM_FAILURE; break;
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

void DataNode::breakPath( const std::string &path, std::string &pathLead, std::string &pathRest )
{
    size_t pos = path.find( '/' );

    pathLead.clear();

    if ( pos != std::string::npos )
    {
        pathLead = path.substr( 0 , pos );
        pathRest = path.substr( pos + 1 );
    }
    else
    {
        pathLead = path;
        pathRest.clear();
    }
}

////////////////////////////////////////////////////////////////////////////////

DataNode* DataNode::createNode( const std::string &name, Type type, DataNode *parent )
{
    std::string name_temp = String::toLower( name );

    DataNode *node = new DataNode();

    node->m_parent = parent;

    node->m_name = name_temp;
    node->m_type = type;

    switch ( node->m_type )
    {
        case Group:  node->m_children.clear();    break;
        case Bool:   node->m_data.bData = false;  break;
        case Int:    node->m_data.iData = 0;      break;
        case Long:   node->m_data.lData = 0L;     break;
        case Float:  node->m_data.fData = 0.0f;   break;
        case Double: node->m_data.dData = 0.0;    break;
    }

    return node;
}

////////////////////////////////////////////////////////////////////////////////

DataNode* DataNode::findNode( const std::string &path )
{
    std::string path_temp = path;

    stripPathSlashes( path_temp );

    if ( path_temp.length() > 0 )
    {
        std::string pathLead;
        std::string pathRest;

        breakPath( path_temp, pathLead, pathRest );

        DataNodes::iterator it;

        it = m_children.find( pathLead );

        if ( it != m_children.end() )
        {
            if ( pathRest.size() > 0 )
            {
                return it->second->findNode( pathRest );
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

void DataNode::stripPathSlashes( std::string &path )
{
    size_t pos;

    // removing leading slashes
    pos = path.find( '/' );

    if ( pos != std::string::npos )
    {
        while ( pos == 0 )
        {
            path   = path.substr( pos + 1 );
            pos    = path.find( '/' );
        }
    }

    // removing trailing slashes
    pos = path.rfind( '/' );

    if ( pos != std::string::npos )
    {
        while ( pos == path.length() - 1 )
        {
            path   = path.substr( 0, pos );
            pos    = path.rfind( '/' );
        }
    }
}
