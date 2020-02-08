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
#ifndef FDM_MAINROTORBE_H
#define FDM_MAINROTORBE_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/models/fdm_MainRotor.h>

#include <fdm/models/fdm_Blade.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Helicopter main rotor model class.
 *
 * This model is based on blade element theory.
 *
 * Flapping angle is positive upwards.
 *
 * Coordinate Systems Used for Rotor Calculations:
 *
 * Rotor Axes System (RAS)
 * Origin of the Rotor Axes System is coincident with the rotor hub center,
 * the x-axis is positive forwards, the y-axis is positive right and z-axis
 * is positive downwards and coincident with the rotor shaft axis.
 *
 * XML configuration file format:
 * @code
 * <main_rotor>
 *   <hub_center> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </hub_center>
 *   <inclination> { [rad] rotor inclination angle (positive if forward) } </inclination>
 *   <number_of_blades> { number of blades } </number_of_blades>
 *   <blade>
 *     { blade data }
 *   </blade>
 * </main_rotor>
 * @endcode
 */
class FDMEXPORT MainRotorBE : public MainRotor
{
public:

    typedef std::vector< Blade* > Blades;

    /** Constructor. */
    MainRotorBE();

    /** Destructor. */
    virtual ~MainRotorBE();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

    /**
     * Returns rotor total inartia about shaft axis.
     * @return [kg*m^2] rotor total inartia about shaft axis
     */
    inline double getInertia() const { return _inertia; }

    inline const Blade* getBlade( int index ) const { return _blades[ index ]; }

protected:

    Blades _blades;             ///< main rotor blades

    double _inertia;            ///< [kg*m^2] rotor inartia about shaft axis

    double _azimuth_prev;       ///< [rad] rotor azimuth position (previous value)
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_MAINROTORBE_H
