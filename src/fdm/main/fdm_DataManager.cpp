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

#include <fdm/main/fdm_DataManager.h>

#include <fdm/fdm_Exception.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

DataManager::DataManager( DataNode *rootNode ) :
    _rootNode( rootNode )
{
    if ( _rootNode == 0 )
    {
        Exception e;

        e.setType( Exception::NullPointer );
        e.setInfo( "Data root node pointer NULL." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

DataManager::DataManager( const DataManager *dataManager ) :
    _rootNode( dataManager->_rootNode )
{
    if ( _rootNode == 0 )
    {
        Exception e;

        e.setType( Exception::NullPointer );
        e.setInfo( "Data root node pointer NULL." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

DataManager::~DataManager() {}

////////////////////////////////////////////////////////////////////////////////

int DataManager::addDataRef( const char *path, DataNode::Type type )
{
    return _rootNode->addNode( path, type );
}

////////////////////////////////////////////////////////////////////////////////

int DataManager::addDataRef( const std::string &path, DataNode::Type type )
{
    return _rootNode->addNode( path.c_str(), type );
}

////////////////////////////////////////////////////////////////////////////////

DataRef DataManager::getDataRef( const char *path )
{
    DataNode *dataNode = _rootNode->getNode( path );

    if ( dataNode != 0 )
    {
        if ( dataNode->getType() == DataNode::Group )
        {
            dataNode = 0;
        }
    }

    return DataRef( dataNode );
}

////////////////////////////////////////////////////////////////////////////////

DataRef DataManager::getDataRef( const std::string &path )
{
    return getDataRef( path.c_str() );
}
