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
#ifndef FDM_HINGEMOMENT_H
#define FDM_HINGEMOMENT_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/main/fdm_Base.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Flight control hinge moment class.
 *
 * <h5>XML configuration file format:</h5>
 * @code
 * <hinge_moment>
 *   <area> { [m^2] control surface area } </area>
 *   <chord> { [m] control surface mean chord (from hinge to trailing edge) } </chord>
 *   <dch_dalpha> { [1/rad] hinge moment coefficient due to angle of attack } </dch_dalpha>
 *   <dch_ddelta> { [1/rad] hinge moment coefficient due to deflection } </dch_ddelta>
 *   <dch_ddelta_t> { [1/rad] hinge moment coefficient due to trim deflection } </dch_ddelta_t>
 * </hinge_moment>
 * @endcode
 *
 * @see Etkin B.: Dynamics of Atmosferic Flight, 1972, p.222
 * @see Paturski Z.: Przewodnik po projektach z Mechaniki Lotu, Projekt nr 9: Rownowaga podluzna samolotu i sily na sterownicy wysokosci, p.IX-3. [in Polish]
 */
class FDMEXPORT HingeMoment : public Base
{
public:

    /** Constructor. */
    HingeMoment();

    /** Destructor. */
    virtual ~HingeMoment();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

    /**
     * Computes hinge moment.
     * @param dynamicPressure [Pa] dynamic pressure
     * @param alpha   [rad] angle of attack
     * @param delta   [rad] control surface deflection
     * @param delta_t [rad] control surface trim deflection
     */
    virtual double getHingeMoment( double dynamicPressure,
                                   double alpha,
                                   double delta,
                                   double delta_t = 0.0 ) const;

protected:

    double m_area;          ///< [m^2] control surface area
    double m_chord;         ///< [m] control surface mean chord (from hinge to trailing edge)

    double m_dch_dalpha;    ///< [1/rad] hinge moment coefficient due to angle of attack
    double m_dch_ddelta;    ///< [1/rad] hinge moment coefficient due to deflection
    double m_dch_ddelta_t;  ///< [1/rad] hinge moment coefficient due to trim deflection
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_HINGEMOMENT_H
