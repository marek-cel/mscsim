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

#include <fdm/main/fdm_Controls.h>
#include <fdm/main/fdm_Aircraft.h>

#include <fdm/fdm_Log.h>

#include <fdm/utils/fdm_String.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Controls::Controls( const Aircraft* aircraft, DataNode *rootNode ) :
    Module ( aircraft, rootNode )
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

            std::string name  = channelNode.getAttribute( "name"  );
            std::string input = channelNode.getAttribute( "input" );

            channel.input = getDataRef( input );

            if ( !channel.input.isValid() )
            {
                Log::w() << "Wrong control channel input: \"" << input << "\"" << std::endl;
            }

            if ( result == FDM_SUCCESS ) result = XmlUtils::read( channelNode, channel.table );
            if ( result == FDM_SUCCESS ) result = _channels.addItem( name, channel );

            if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, channelNode );

            channelNode = channelNode.getNextSiblingElement( "control_channel" );
        }

        if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Controls::initialize()
{
    update();
}

////////////////////////////////////////////////////////////////////////////////

void Controls::update()
{
    for ( Channels::iterator it = _channels.begin(); it != _channels.end(); ++it )
    {
        Channel &ch = (*it).second;

        if ( ch.input.isValid() )
            ch.output = ch.table.getValue( ch.input.getValue() );
        else
            ch.output = ch.table.getValue( 0.0 );
    }
}
