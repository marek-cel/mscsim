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
#ifndef UH60_AFCS_H
#define UH60_AFCS_H

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief UH-60 Automatic Flight Control System (AFCS) class.
 *
 * @see Howlett J.: UH-60A Black Hawk Engineering Simulation Program. Volume 1: Mathematical Model, NASA-CR-166309, 1981
 * @see Howlett J.: UH-60A Black Hawk Engineering Simulation Program. Volume II - Background Report, NASA-CR-166310, 1981
 * @see Hilbert K.: A Mathematical Model of the UH-60 Helicopter, NASA-TM-85890, 1984
 */
class UH60_AFCS
{
public:

    /** Constructor. */
    UH60_AFCS();

    /** Destructor. */
    virtual ~UH60_AFCS();
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // UH60_AFCS_H
