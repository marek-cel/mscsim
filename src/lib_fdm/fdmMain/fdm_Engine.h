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
#ifndef FDM_ENGINE_H
#define FDM_ENGINE_H

////////////////////////////////////////////////////////////////////////////////

#include <fdmMain/fdm_Base.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Engine base class.
 */
class FDMEXPORT Engine : public Base
{
public:

    /** Engine state enum. */
    enum State
    {
        Stopped  = 0,   ///< engine stopped
        Starting = 1,   ///< engine starting
        Running  = 2    ///< engine running
    };

    /** Constructor. */
    Engine() : m_state ( Stopped ) {}

    /** Constructor. */
    virtual ~Engine() {}

    /**
     * Returns engine state.
     * @return engine state
     */
    inline State getState() const
    {
        return m_state;
    }

protected:

    State m_state;              ///< engine state
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_ENGINE_H
