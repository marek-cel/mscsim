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
#ifndef FDM_LANDINGGEAR_H
#define FDM_LANDINGGEAR_H

////////////////////////////////////////////////////////////////////////////////

#include <vector>

#include <fdm/main/fdm_Module.h>

#include <fdm/models/fdm_Wheel.h>

#include <fdm/utils/fdm_Map.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Landing gear base class.
 */
class FDMEXPORT LandingGear : public Module
{
public:

    /** Wheel and input data reference struct. */
    struct WheelAndInput
    {
        DataRef input;      ///< input data reference
        Wheel   wheel;      ///< wheel model object
    };

    typedef Map< std::string, WheelAndInput > Wheels;

    /** @brief Constructor. */
    LandingGear( const Aircraft *aircraft, Input *input );

    /** @brief Destructor. */
    virtual ~LandingGear();

    /**
     * @brief Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode ) = 0;

    /** @brief Initializes landing gear. */
    virtual void initialize();

    /** @brief Computes force and moment. */
    virtual void computeForceAndMoment() = 0;

    /** @brief Updates landing gear. */
    virtual void update();

    inline const Vector3& getFor_BAS() const { return _for_bas; }
    inline const Vector3& getMom_BAS() const { return _mom_bas; }

    inline double getPosition() const { return _position; }

    inline bool getOnGround() const { return _onGround; }

protected:

    Vector3 _for_bas;           ///< [N] total force vector expressed in BAS
    Vector3 _mom_bas;           ///< [N*m] total moment vector expressed in BAS

    double _ctrlAngle;          ///< [rad] wheel turn angle

    double _brake_l;            ///< <0.0;1.0> normalized brake force (left brake group)
    double _brake_r;            ///< <0.0;1.0> normalized brake force (right brake group)

    double _position;           ///< <0.0;1.0> normalized landing gear position

    bool _antiskid;             ///< specifies if antiskid is enabled
    bool _steering;             ///< specifies if steering is enabled

    bool _onGround;             ///< specifies if aircraft is on the ground

    DataRef _inputABS;          ///< ABS input data reference
    DataRef _inputNWS;          ///< NWS input data reference

    /**
     * @brief Returns landing gear strut and ground plane intersection point expressed in BAS.
     * @return [m] landing gear strut and ground plane intersection point expressed in BAS
     */

    /**
     * @brief Returns landing gear strut and ground plane intersection point expressed in BAS.
     * @param r_a_bas [m] landing gear strut attachment point expressed in BAS
     * @param r_u_bas [m] landing gear strut unloaded wheel expressed in BAS
     * @param r_c_bas [m] intersection coordinates expressed in BAS
     * @param n_c_bas [-] intersection normal vector
     * @return returns true if intersection occurs, false otherwise
     */
    virtual bool getIsect( const Vector3 &r_a_bas, const Vector3 &r_u_bas,
                           Vector3 *r_c_bas, Vector3 *n_c_bas );

    virtual int readWheelsData( XmlNode &dataNode, Wheels &wheels );

private:

    /** Using this constructor is forbidden. */
    LandingGear( const LandingGear & ) : Module( FDM_NULLPTR, FDM_NULLPTR ) {}
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_LANDINGGEAR_H
