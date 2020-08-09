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
#ifndef FDM_WHEEL_H
#define FDM_WHEEL_H

////////////////////////////////////////////////////////////////////////////////

#include <vector>

#include <fdm/main/fdm_Module.h>

#include <fdm/utils/fdm_Vector3.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Landing gear wheel class.
 *
 * XML configuration file format:
 * @code
 * <wheel [steerable="{ 0|1 }"] [caster="{ 0|1 }"] [brake_group="{ 0|1|2 }]">
 *   <attachment_point> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </attachment_point>
 *   <unloaded_wheel> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </unloaded_wheel>
 *   <stiffness> { [N/m] strut stiffness (linear spring) coefficient } </stiffness>
 *   <damping> { [N/(m/s)] strut damping coefficient  } </damping>
 *   <friction_static> { [-] static friction coefficient } </friction_static>
 *   <friction_kinetic> { [-] kinetic friction coefficient } </friction_kinetic>
 *   <friction_rolling> { [-] rolling friction coefficient } </friction_rolling>
 *   [<max_angle> { [rad] max steering angle } </max_angle>]
 *   [<d_max> { [m] maximum distance for static friction spring like model } </d_max>]
 *   [<v_max> { [m/s] maximum velocity for continuous friction model } </v_max>]
 * </wheel>
 * @endcode
 *
 * Optional elements: "max_angle", "vel_break"
 *
 * @see J. O’Rourke, Computational Geometry in C, 1998
 * @see K. Studzinski: Samochod. Teoria, konstrukcja i obliczenia, 1980 [in Polish]
 * @see P.R. Dahl: A Solid Friction Model, ADA041920, 1968
 * @see V. van Geffen: A study of friction models and friction compensation, 2009
 */
class FDMEXPORT Wheel
{
public:

    /** Wheels container class. */
    class Wheels
    {
    public:

        typedef std::map< std::string, Wheel >::iterator iterator;

        /** Constructor. */
        Wheels();

        /** Destructor. */
        virtual ~Wheels();

        /**
         * Adds wheel.
         * @param name
         * @param wheel
         * @return returns FDM_SUCCESS on success and FDM_FAILURE on failure
         */
        int addWheel( const char *name, Wheel wheel );

        /**
         * Returns wheel by name.
         * @param name wheel name
         * @return channel
         */
        Wheel* getWheelByName( const char *name );

        inline iterator begin() { return _wheels.begin(); }
        inline iterator end()   { return _wheels.end();   }

    private:

        std::map< std::string, Wheel > _wheels;
    };

    /** Brake group. */
    enum BrakeGroup
    {
        None = 0,               ///< none
        Left,                   ///< left brake group
        Right                   ///< right brake group
    };

    /**
     * Returns landing gear strut and ground plane intersection point.
     * @see O'Rourke J.: Computational Geometry in C, 1998, p.226
     * @see http://paulbourke.net/geometry/pointlineplane/
     * @param b [m] landing gear strut attachment point coordinates (beginning)
     * @param e [m] landing gear unloaded wheel coordinates (end)
     * @param r [m] point on the ground plane coordinates
     * @param n [-] ground plane normal vector
     * @return [m] landing gear strut and ground plane intersection point
     */
    static Vector3 getIntersection( const Vector3 &b, const Vector3 &e,
                                    const Vector3 &r, const Vector3 &n );

    /**
     * Returns Pacejka "Magic Formula" coefficient.
     * @see https://en.wikipedia.org/wiki/Hans_B._Pacejka#The_Pacejka_%22Magic_Formula%22_tire_models
     * @see https://www.mathworks.com/help/physmod/sdl/ref/tireroadinteractionmagicformula.html
     * @param kappa [-] slip parameter (v_slip/v_roll)
     * @param b b coefficient
     * @param c c coefficient
     * @param d d coefficient
     * @param e e coefficient
     * @return Pacejka "Magic Formula" coefficient
     */
    static double getPacejkaCoef( double kappa,
                                  double b = 10.0, double c = 1.9,
                                  double d = 1.0,  double e = 0.97 );

    /** Constructor. */
    Wheel();

    /** Destructor. */
    virtual ~Wheel();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

    /**
     * Computes force and moment.
     * @param vel_bas [m/s] aircraft linear velocity expressed in BAS
     * @param omg_bas [rad/s] aircraft angular velocity expressed in BAS
     * @param r_c_bas [m] contact point coordinates expressed in BAS
     * @param n_c_bas [-] contact point normal vector expressed in BAS
     * @param steering
     * @param antiskid
     * @param surf_coef
     */
    virtual void computeForceAndMoment( const Vector3 &vel_bas,
                                        const Vector3 &omg_bas,
                                        const Vector3 &r_c_bas,
                                        const Vector3 &n_c_bas,
                                        bool steering, bool antiskid,
                                        double surf_coef = 1.0 );

    /**
     * Integrates wheel model.
     * @param timeStep [s] time step
     * @param vel_bas
     * @param omg_bas
     * @param r_c_bas
     * @param n_c_bas
     * @param steering
     */
    virtual void integrate( double timeStep,
                            const Vector3 &vel_bas,
                            const Vector3 &omg_bas,
                            const Vector3 &r_c_bas,
                            const Vector3 &n_c_bas,
                            bool steering );

    /**
     * Update wheel model.
     * @param delta
     * @param brake
     */
    virtual void update( double delta, double brake );

    inline const Vector3& getFor_BAS() const { return _for_bas; }
    inline const Vector3& getMom_BAS() const { return _mom_bas; }

    inline Vector3 getRa_BAS() const { return _r_a_bas; }
    inline Vector3 getRu_BAS() const { return _r_u_bas; }

    inline BrakeGroup getBrakeGroup() const { return _brakeGroup; }

    inline void setInput( const double *input ) { _input = input; }

    inline bool isInputValid() const { return _input != FDM_NULLPTR; }

    inline double getInputValue() const { return (*_input); }

protected:

    const double *_input;   ///< wheel input (0.0 - retracted, 1.0 - extended)

    Vector3 _for_bas;       ///< [N] total force vector expressed in BAS
    Vector3 _mom_bas;       ///< [N*m] total moment vector expressed in BAS

    Vector3 _r_a_bas;       ///< [m] strut attachment point coordinates expressed in BAS
    Vector3 _r_u_bas;       ///< [m] unloaded wheel coordinates expressed in BAS

    Vector3 _r_0_wgs;

    double _k;              ///< [N/m] strut stiffness (linear spring) coefficient
    double _c;              ///< [N/(m/s)] strut damping coefficient

    double _mu_s;           ///< [-] coefficient of static friction
    double _mu_k;           ///< [-] coefficient of kinetic friction
    double _mu_r;           ///< [-] coefficient of rolling friction

    double _delta_max;      ///< [rad] max turn angle

    double _d_max;          ///< [m] maximum distance for static friction spring like model
    double _v_max;          ///< [m/s] maximum velocity for continuous friction model

    bool _steerable;        ///< specifies if wheel is steerable
    bool _caster;           ///< specifies if wheel is caster

    BrakeGroup _brakeGroup; ///< brake group

    double _d_roll;         ///< [m] roll direction distance for static friction spring like model
    double _d_slip;         ///< [m] slip direction distance for static friction spring like model

    double _delta;          ///< [rad] wheel turn angle
    double _brake;          ///< [0.0,1.0] normalized brake force

    /**
     * Calculates wheel variables.
     * @param vel_bas
     * @param omg_bas
     * @param r_c_bas
     * @param n_c_bas
     * @param steering
     * @param dir_lon_bas
     * @param dir_lat_bas
     * @param cosDelta
     * @param sinDelta
     * @param v_norm
     * @param v_roll
     * @param v_slip
     */
    void calculateVariables( const Vector3 &vel_bas,
                             const Vector3 &omg_bas,
                             const Vector3 &r_c_bas,
                             const Vector3 &n_c_bas,
                             bool steering,
                             Vector3 *dir_lon_bas,
                             Vector3 *dir_lat_bas,
                             double *cosDelta,
                             double *sinDelta,
                             double *v_norm,
                             double *v_roll,
                             double *v_slip );
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_WHEEL_H
