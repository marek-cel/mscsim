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
#ifndef DATA_H
#define DATA_H

////////////////////////////////////////////////////////////////////////////////

#define MAX_BLADES 4

#define VECT_MAIN 3
#define VECT_SPAN 30

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Simulation data access class.
 */
class Data
{
public:

    enum Phase
    {
        Init = 0,
        Work,
        Pause,
        Stop
    };

    /** Blade data. */
    struct Blade
    {
        double beta;                ///< [rad] flapping angle
        double theta;               ///< [rad] feathering angle
    };

    /** Rotor data. */
    struct Rotor
    {
        enum Direction
        {
            CW = 0,
            CCW
        };

        double omega;               ///< [rad/s] main rotor speed
        double azimuth;             ///< [rad] main rotor rotation angle (azimuth)
        double coningAngle;         ///< [rad] main rotor coning angle
        double diskRoll;            ///< [rad] main rotor disk roll angle
        double diskPitch;           ///< [rad] main rotor disk pitch angle
        double collective;          ///< [rad] main rotor collective pitch angle
        double cyclicLon;           ///< [rad] main rotor longitudinal cyclic pitch angle
        double cyclicLat;           ///< [rad] main rotor lateral cyclic pitch angle

        Direction direction;        ///<
    };

    /** State data. */
    struct State
    {
        double bas_u;               ///<
        double bas_v;               ///<
        double bas_w;               ///<

        double bas_p;               ///<
        double bas_q;               ///<
        double bas_r;               ///<

        double ned_phi;             ///< [rad]
        double ned_tht;             ///< [rad]
        double ned_psi;             ///< [rad]

        double wind_vel;            ///< [m/s]
        double wind_dir;            ///< [rad]
    };

    /** Other data. */
    struct Other
    {
        struct Vector
        {
            bool visible;

            double b_x_enu;         ///<
            double b_y_enu;         ///<
            double b_z_enu;         ///<

            double v_x_enu;         ///<
            double v_y_enu;         ///<
            double v_z_enu;         ///<

            char label[ 64 ];       ///<


        };

        Vector main[ VECT_MAIN ];   ///<
        Vector span[ VECT_SPAN ];   ///<

        double scale_main;          ///<
        double scale_span;          ///<

        bool show_vectors_main;     ///<
        bool show_vectors_span;     ///<
        bool show_blades_datum;     ///<
        bool show_blades_trace;     ///<
    };

    /** Data struct. */
    struct DataBuf
    {
        Blade blade[ MAX_BLADES ];  ///<
        Rotor rotor;                ///<
        State state;                ///<
        Other other;                ///<

        Phase phase;                ///<

        double time_step;           ///<
        double time_coef;           ///<

        int blades_count;           ///<

        bool test;                  ///<
    };

private:

    /**
     * You should use static function instance() due to get refernce
     * to Data class instance.
     */
    Data() {}

    /** Using this constructor is forbidden. */
    Data( const Data & ) {}

public:

    /** Returns pointer to the common data object. */
    static inline DataBuf* get() { return &_data; }

    /** Destructor. */
    virtual ~Data() {}

private:

    static DataBuf _data;   ///< data
};

////////////////////////////////////////////////////////////////////////////////

#endif // DATA_H
