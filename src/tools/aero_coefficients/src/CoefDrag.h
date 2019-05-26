#ifndef COEFDRAG_H
#define COEFDRAG_H

////////////////////////////////////////////////////////////////////////////////

/**
 * Drag coefficient approximation class.
 * @see Takahashi M.: A Flight-Dynamic Helicopter Mathematical Model with a Single Flap-Lag-Torsion Main Rotor, NASA, TM-102267, p.67
 */
class CoefDrag
{
public:

    CoefDrag( double cd_0, double cd_1, double cd_2, double cd_3, double cd_4, double cd_5,
              double ad_1, double ad_2, double ad_3, double ad_4 );

    double get( double aoa );

private:

    double m_cd_0;      ///< [-]
    double m_cd_1;      ///< [-]
    double m_cd_2;      ///< [-]
    double m_cd_3;      ///< [-]
    double m_cd_4;      ///< [-]
    double m_cd_5;      ///< [-]

    double m_ad_1;      ///< [rad]
    double m_ad_2;      ///< [rad]
    double m_ad_3;      ///< [rad]
    double m_ad_4;      ///< [rad]

    double m_ad_1_2;    ///< [rad^2]
    double m_ad_2_2;    ///< [rad^2]

    double m_div_d1_0;  ///<
    double m_div_d1_1;  ///<
    double m_div_d1_2;  ///<

    double m_div_d2_2;  ///<
    double m_div_d2_3;  ///<
    double m_div_d2_4;  ///<
    double m_div_d2_5;  ///<
};

////////////////////////////////////////////////////////////////////////////////

#endif // COEFDRAG_H
