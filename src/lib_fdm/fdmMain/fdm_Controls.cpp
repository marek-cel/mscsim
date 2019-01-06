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

#include <fdmMain/fdm_Aircraft.h>

#include <fdmUtils/fdm_String.h>
#include <fdmXml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Controls::Controls( const Aircraft* aircraft ) :
    DataManager( aircraft ),
    m_aircraft ( aircraft )
{}

////////////////////////////////////////////////////////////////////////////////

Controls::~Controls() {}

////////////////////////////////////////////////////////////////////////////////

void Controls::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        XmlNode channelNode = dataNode.getFirstChildElement( "control_channel" );

        while ( result == FDM_SUCCESS && channelNode.isValid() )
        {
            Channel channel;

            channel.output = 0.0;
            channel.name = channelNode.getAttribute( "name" );

            std::string drName = channelNode.getAttribute( "input" );

            if ( result == FDM_SUCCESS ) result = XmlUtils::read( channelNode, channel.table );
            if ( result == FDM_SUCCESS ) result = addDataRef( drName, DataNode::Double );

            if ( result == FDM_SUCCESS )
            {
                channel.drInput = getDataRef( drName );

                if ( channel.drInput.isValid() )
                {
                    m_channels.push_back( channel );
                }
            }
            else
            {
                Exception e;

                e.setType( Exception::FileReadingError );
                e.setInfo( "Error reading XML file. " + XmlUtils::getErrorInfo( dataNode ) );

                FDM_THROW( e );
            }

            channelNode = channelNode.getNextSiblingElement( "control_channel" );
        }

        if ( result != FDM_SUCCESS )
        {
            Exception e;

            e.setType( Exception::FileReadingError );
            e.setInfo( "Error reading XML file. " + XmlUtils::getErrorInfo( dataNode ) );

            FDM_THROW( e );
        }
    }
    else
    {
        Exception e;

        e.setType( Exception::FileReadingError );
        e.setInfo( "Error reading XML file. " + XmlUtils::getErrorInfo( dataNode ) );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Controls::update()
{
    for ( Channels::iterator it = m_channels.begin(); it != m_channels.end(); it++ )
    {
        (*it).output = (*it).table.getValue( (*it).drInput.getDatad() );
    }
}

////////////////////////////////////////////////////////////////////////////////

Controls::Channel* Controls::getChannelByName( const std::string &name )
{
    for ( Channels::iterator it = m_channels.begin(); it != m_channels.end(); it++ )
    {
        if ( 0 == String::icompare( name, (*it).name ) )
        {
            return &(*it);
        }
    }

    return 0;
}
