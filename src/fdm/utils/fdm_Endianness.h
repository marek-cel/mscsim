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
#ifndef FDM_ENDIANNESS_H
#define FDM_ENDIANNESS_H

////////////////////////////////////////////////////////////////////////////////

#ifdef _LINUX_
#   include <netinet/in.h>
#endif

#include <fdm/fdm_Defines.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Endianness utilities class.
 */
class FDMEXPORT Endianness
{
public:

    /**
     * @brief Converts values between host and network byte order.
     * @param val value expressed in host byte order
     * @return value expressed in network byte order
     */
    static unsigned int hostToNet( unsigned int val );

    /**
     * @brief Converts values between host and network byte order.
     * @param val value expressed in host byte order
     * @return value expressed in network byte order
     */
    static unsigned short hostToNet( unsigned short val );

    /**
     * @brief Converts values between host and network byte order.
     * @param val value expressed in host byte order
     * @return value expressed in network byte order
     */
    static double hostToNet( double val );

    /**
     * @brief Converts values between host and network byte order.
     * @param val value expressed in host byte order
     * @return value expressed in network byte order
     */
    static float hostToNet( float val );
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_ENDIANNESS_H
