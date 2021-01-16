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
#ifndef XH_FUSELAGE_H
#define XH_FUSELAGE_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/models/fdm_Fuselage.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief X/H fuselage class.
 */
class XH_Fuselage : public Fuselage
{
public:

    /** Constructor. */
    XH_Fuselage();

    /** Destructor. */
    virtual ~XH_Fuselage();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( XmlNode &dataNode );

private:

    Table1 _cx_beta;            ///< [-] incremental drag coefficient vs [rad] angle of sideslip
    Table1 _cz_beta;            ///< [-] incremental lift coefficient vs [rad] angle of sideslip
    Table1 _cm_beta;            ///< [-] incremental pitching moment coefficient vs [rad] angle of sideslip

    /**
     * Computes drag coefficient.
     * @param angleOfAttack [rad] angle of attack
     * @return [-] drag coefficient
     */
    double getCx( double angleOfAttack ) const;

    /**
     * Computes lift coefficient.
     * @param angleOfAttack [rad] angle of attack
     * @return [-] lift coefficient
     */
    double getCz( double angleOfAttack ) const;

    /**
     * Computes pitching moment coefficient.
     * @param angleOfAttack [rad] angle of attack
     * @return [-] pitching moment coefficient
     */
    double getCm( double angleOfAttack ) const;
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // XH_FUSELAGE_H
