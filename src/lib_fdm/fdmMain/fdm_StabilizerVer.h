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
#ifndef FDM_STABILIZERVER_H
#define FDM_STABILIZERVER_H

////////////////////////////////////////////////////////////////////////////////

#include <fdmMain/fdm_Stabilizer.h>
#include <fdmUtils/fdm_Table.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Vertical stabilizer class.
 *
 * <h5>XML configuration file format:</h5>
 * @code
 * <vertical_stabilizer>
 *   <position> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </position>
 *   <area> { [m^2] stabilizer area } </area>
 *   <cx>
 *     { [deg] stabilizer sidelip angle } { [-] stabilizer drag coefficient }
 *     ... { more entries }
 *   </cx>
 *   <cy>
 *     { [deg] stabilizer sidelip angle } { [-] stabilizer side force coefficient }
 *     ... { more entries }
 *   </cy>
 * </vertical_stabilizer>
 * @endcode
 */
class FDMEXPORT StabilizerVer : public Stabilizer
{
public:

    /** Constructor. */
    StabilizerVer();

    /** Destructor. */
    virtual ~StabilizerVer();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

protected:

    Table m_cx;                 ///< [-] drag coefficient vs sidelip angle
    Table m_cy;                 ///< [-] side force coefficient vs sidelip angle
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_STABILIZERVER_H
