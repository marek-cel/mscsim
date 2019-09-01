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
#ifndef FDM_CONTROLS_H
#define FDM_CONTROLS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Base.h>

#include <fdm/utils/fdm_Table.h>
#include <fdm/xml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class Aircraft; ///< aircraft class forward declaration

/**
 * @brief Controls model base class.
 *
 * <p>Input data reference is created for each control channel. Name of
 * this data reference is "input/controls/channel_name".</p>
 *
 * <h5>XML configuration file format:</h5>
 * @code
 * <controls>
 *   <control_channel input="{ channel input }">
 *     { channel input } { channel output }
 *     ... { more entries }
 *   </control_channel>
 *   ... { more entries }
 * </controls>
 * @endcode
 *
 * @see ISO 1151-4:1994
 * @see ISO 1151-6:1982
 */
class FDMEXPORT Controls : public Base
{
public:

    /** Control channel data. */
    struct Channel
    {
        const double *input;    ///< channel input
        Table table;            ///< channel input vs output data
        double output;          ///< channel output
    };

    typedef std::map< std::string, Channel > Channels;

    /** Constructor. */
    Controls( const Aircraft* aircraft );

    /** Destructor. */
    virtual ~Controls();

    /** Initializes controls. */
    virtual void init();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

    /** Updates controls. */
    virtual void update();

protected:

    const Aircraft *_aircraft;  ///< aircraft model main object
    Channels _channels;         ///< control channels

    /**
     * Returns channel by name.
     * @param name channel name
     * @return channel
     */
    virtual Channel* getChannelByName( const std::string &name );

private:

    /** Using this constructor is forbidden. */
    Controls( const Controls & ) {}
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_CONTROLS_H
