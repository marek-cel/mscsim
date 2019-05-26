#include "CoefLift.h"

#include <iostream>
#include <math.h>

////////////////////////////////////////////////////////////////////////////////

CoefLift::CoefLift( double cl_s, double cl_1, double cl_2,
                    double al_s, double al_1, double al_2 )
{
    m_cl_0 = 0.0;
    m_cl_s = cl_s;
    m_cl_1 = cl_1;
    m_cl_2 = cl_2;

    m_al_s = al_s;
    m_al_1 = al_1;
    m_al_2 = al_2;

    m_dcl_da_s = m_cl_s / m_al_s;
    m_dcl_da_1 = ( m_cl_1 - m_cl_s ) / ( m_al_1 - m_al_s );

    m_div_l1_1 = ( m_al_1 - m_al_2 ) * ( m_al_1 - M_PI_2 );
    m_div_l1_2 = ( m_al_2 - m_al_1 ) * ( m_al_2 - M_PI_2 );
}

////////////////////////////////////////////////////////////////////////////////

double CoefLift::get( double aoa )
{
    double coef = ( aoa < -M_PI_2 || aoa > M_PI_2 ) ? -1.0 : 1.0;

    if ( aoa < -M_PI_2 ) aoa = -M_PI - aoa;
    if ( aoa >  M_PI_2 ) aoa =  M_PI - aoa;

    double cl = m_cl_0;

    if ( aoa < -m_al_1 )
    {
        cl =  - m_cl_1 * ( aoa + m_al_2 ) * ( aoa + M_PI_2 ) / m_div_l1_1
              - m_cl_2 * ( aoa + m_al_1 ) * ( aoa + M_PI_2 ) / m_div_l1_2
              + m_cl_0;
    }
    else if ( aoa < -m_al_s )
    {
        cl =  m_dcl_da_1 * ( aoa + m_al_s ) - m_cl_s + m_cl_0;
    }
    else if ( aoa < m_al_s )
    {
        cl =  m_dcl_da_s * aoa + m_cl_0;
    }
    else if ( aoa < m_al_1 )
    {
        cl =  m_dcl_da_1 * ( aoa - m_al_s ) + m_cl_s + m_cl_0;
    }
    else
    {
        cl =  m_cl_1 * ( aoa - m_al_2 ) * ( aoa - M_PI_2 ) / m_div_l1_1
            + m_cl_2 * ( aoa - m_al_1 ) * ( aoa - M_PI_2 ) / m_div_l1_2
            + m_cl_0;
    }

    return coef * cl;
}
