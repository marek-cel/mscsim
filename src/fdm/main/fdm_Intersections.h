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
#ifndef FDM_INTERSECTIONS_H
#define FDM_INTERSECTIONS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/utils/fdm_Vector3.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Intersections interface class.
 */
class FDMEXPORT Intersections
{
public:

    /** @brief Constructor. */
    Intersections();

    /** @brief Destructor. */
    virtual ~Intersections();

    /** */
    virtual void update( double lat, double lon );

    /**
     * @brief Returns ground elevation above mean sea level.
     * @param lat [rad] input latitude
     * @param lon [rad] input longitude
     * @return [m] ground elevation above mean sea level
     */
    virtual double getElevation( double lat, double lon ) const;

    /**
     * @brief Gets intersection.
     * @param b [m] beginning of intersection line expressed in WGS
     * @param e [m] end of intersection line expressed in WGS
     * @param r [m] intersection point coordinates expressed in WGS
     * @param n [-] intersection normal vector expressed in WGS
     * @param update specifies if ground intersection data should be updated
     * @return FDM_SUCCESS on success or FDM_FAILURE on failure
     */
    virtual int getIntersection( const Vector3 &b, const Vector3 &e,
                                 Vector3 *r, Vector3 *n, bool update = false ) const;

    /**
     * @brief Checks intersection.
     * @param b [m] beginning of intersection line expressed in WGS
     * @param e [m] end of intersection line expressed in WGS
     * @param update specifies if ground intersection data should be updated
     * @return true if there is an intersection, false otherwise
     */
    virtual bool isIntersection( const Vector3 &b, const Vector3 &e,
                                 bool update = false ) const;

    /**
     * @brief Returns ground normal vector expressed in WGS.
     * @param lat [rad] input latitude
     * @param lon [rad] input longitude
     * @param update specifies if ground intersection data should be updated
     * @return ground normal vector expressed in WGS
     */
    virtual Vector3 getNormal( double lat, double lon, bool update = false ) const;

    void setGroundWGS( const Vector3 &ground_wgs ) { _ground_wgs = ground_wgs; }
    void setNormalWGS( const Vector3 &normal_wgs ) { _normal_wgs = normal_wgs; }

protected:

    bool _inited;               ///< specifies if intersections are initialized

    Vector3 _ground_wgs;        ///< [m] ground intersection expressed in WGS
    Vector3 _normal_wgs;        ///< ground normal vector
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_INTERSECTIONS_H
