#ifndef COEFLIFT_H
#define COEFLIFT_H

////////////////////////////////////////////////////////////////////////////////

/**
 * Lift coefficient approximation class.
 * @see Takahashi M.: A Flight-Dynamic Helicopter Mathematical Model with a Single Flap-Lag-Torsion Main Rotor, NASA, TM-102267, p.68
 */
class CoefLift
{
public:

    CoefLift( double cl_s, double cl_1, double cl_2,
              double al_s, double al_1, double al_2 );

    double get( double aoa );

private:

    double m_cl_0;      ///< [-]

    double m_cl_s;      ///< [-]
    double m_cl_1;      ///< [-]
    double m_cl_2;      ///< [-]

    double m_al_s;      ///< [rad]
    double m_al_1;      ///< [rad]
    double m_al_2;      ///< [rad]

    double m_dcl_da_s;  ///< [1/rad]
    double m_dcl_da_1;  ///< [1/rad]

    double m_div_l1_1;  ///<
    double m_div_l1_2;  ///<
};

////////////////////////////////////////////////////////////////////////////////

#endif // COEFLIFT_H
