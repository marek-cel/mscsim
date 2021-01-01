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
#ifndef FDM_GEOM_H
#define FDM_GEOM_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/utils/fdm_Vector3.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Geometric utilities.
 */
class FDMEXPORT Geom
{
public:

    /**
     * @brief Checks if intersection occurs.
     * @param b segment beginning
     * @param e segment end
     * @param r any point on the plane coordinates
     * @param n plane normal vector
     * @return true if there is an intersection, false otherwise
     */
    static bool isIsect( const Vector3 &b, const Vector3 &e,
                         const Vector3 &r, const Vector3 &n );

    /**
     * @brief Returns segment and plane intersection point.
     * @see O'Rourke J.: Computational Geometry in C, 1998, p.226
     * @see http://paulbourke.net/geometry/pointlineplane/
     * @param b segment beginning
     * @param e segment end
     * @param r any point on the plane coordinates
     * @param n plane normal vector
     * @return intersection point
     */
    static Vector3 getIsect( const Vector3 &b, const Vector3 &e,
                             const Vector3 &r, const Vector3 &n );

};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_GEOM_H
