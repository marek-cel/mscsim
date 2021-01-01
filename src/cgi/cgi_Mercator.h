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
#ifndef CGI_MERCATOR_H
#define CGI_MERCATOR_H

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief Mercator projection coordinates computation class.
 *
 * @see Evenden G.: libproj4: A Comprehensive Library of Cartographic Projection Functions (Preliminary Draft), 2005
 */
class Mercator
{
public:

    static const double _max_x;     ///< [m] maximum Mercator x-coordinate for longitude 180 deg
    static const double _max_y;     ///< [m] maximum Mercator y-coordinate for latitude 85 deg

    /**
     * @brief Computes geodetic latitude.
     * @param y [m] Mercator y-coordinate
     * @param max_error maximum error (solve condition)
     * @param max_iterations maximum number of iterations
     * @return geodetic latitude [rad]
     */
    static double lat( double y, double max_error = 1.0e-9,
                       unsigned int max_iterations = 10 );

    /**
     * @brief Computes geodetic longitude.
     * @param x [m] Mercator x-coordinate
     * @return geodetic longitude [rad]
     */
    static double lon( double x );

    /**
     * @brief Computes Mercator x-coordinate.
     * @param lon [rad] geodetic longitude
     * @return Mercator x-coordinate [m]
     */
    static double x( double lon );

    /**
     * @brief Computes Mercator y-coordinate.
     * @param lat [rad] geodetic latitude
     * @return Mercator y-coordinate [m]
     */
    static double y( double lat );

    /**
     * @brief Computes ellipsoid parallel radius.
     * @param lat_ts [rad] geodetic latitude of true scale
     * @return parallel radius [m]
     */
    static double k0( double lat_ts );

    /**
     * @brief Computes Isometric Latitude kernel.
     * @param lat [rad] geodetic latitude
     * @return Isometric Latitude kernel
     */
    static double t( double lat );

    /**
     * @brief Computes geodetic latitude from the isometric latitude.
     * @param t isometric latitude
     * @param max_error maximum error (solve condition)
     * @param max_iterations maximum number of iterations
     * @return geodetic latitude [rad]
     */
    static double t_inv( double t, double max_error = 1.0e-9,
                         unsigned int max_iterations = 10 );
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_MERCATOR_H
