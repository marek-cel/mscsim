/***************************************************************************//**
 *
 * @author Marek M. Cel <marekcel@marekcel.pl>
 *
 * @section LICENSE
 *
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
#ifndef PW5_FDM_H
#define PW5_FDM_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_FDM.h>

#include <fdm_pw5/pw5_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/** PW-5 flight dynamics model wrapper class. */
class PW5_FDM : public FDM
{
public:

    /** Constructor. */
    PW5_FDM( const DataInp *dataInpPtr, DataOut *dataOutPtr, bool verbose = false );

    /** Destructor. */
    virtual ~PW5_FDM();

private:

    PW5_Aircraft *_aircraft;        ///< aircraft model

    virtual void updateDataOut();
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // PW5_FDM_H
