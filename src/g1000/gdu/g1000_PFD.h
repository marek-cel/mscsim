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
#ifndef G1000_PFD_H
#define G1000_PFD_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/Geode>

#include <g1000/gdu/g1000_GDU.h>

#include <g1000/gdu/g1000_ADI.h>
#include <g1000/gdu/g1000_ALT.h>
#include <g1000/gdu/g1000_ASI.h>
#include <g1000/gdu/g1000_HSI.h>
#include <g1000/gdu/g1000_VSI.h>

////////////////////////////////////////////////////////////////////////////////

namespace g1000
{

/**
 * @brief PFD (Primary Flight Display) class.
 */
class PFD : public GDU
{
public:

    static const osg::Vec3 _colorBars;
    static const double _z_bars;

    /** Constructor. */
    PFD();

    /** Destructor. */
    virtual ~PFD();

    /** Initializes PFD. */
    void init( const std::string &file );

    /** Updates PFD. */
    void update( const Data &data );

private:

    ADI *_adi;              ///< attitude director indicator
    ALT *_alt;              ///< altimeter
    ASI *_asi;              ///< airspeed indicator
    HSI *_hsi;              ///< horizontal situation indicator
    VSI *_vsi;              ///< vertical speed indicator

    void createLowerBar( XmlNode &node );
    void createUpperBar( XmlNode &node );
};

} // end of g1000 namespace

////////////////////////////////////////////////////////////////////////////////

#endif // G1000_PFD_H
