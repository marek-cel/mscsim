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
#ifndef FDM_PROPULSION_H
#define FDM_PROPULSION_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/main/fdm_Module.h>

#include <fdm/utils/fdm_Vector3.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Propulsion base class.
 */
class FDMEXPORT Propulsion : public Module
{
public:

    /** @brief Constructor. */
    Propulsion( const Aircraft *aircraft, Input *input );

    /** @brief Destructor. */
    virtual ~Propulsion();

    /**
     * @brief Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode ) = 0;

    /** @brief Initializes propulsion. */
    virtual void initialize();

    /** @brief Computes force and moment. */
    virtual void computeForceAndMoment() = 0;

    /** @brief Updates propulsion. */
    virtual void update() = 0;

    inline const Vector3& getFor_BAS() const { return _for_bas; }
    inline const Vector3& getMom_BAS() const { return _mom_bas; }

protected:

    Vector3 _for_bas;           ///< [N] total force vector expressed in BAS
    Vector3 _mom_bas;           ///< [N*m] total moment vector expressed in BAS

private:

    /** Using this constructor is forbidden. */
    Propulsion( const Propulsion & ) : Module( FDM_NULLPTR, FDM_NULLPTR ) {}
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_PROPULSION_H
