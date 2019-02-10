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

#include <fdm_f16c/f16c_FLCS.h>

#include <algorithm>

#include <fdmSys/fdm_Inertia.h>
#include <fdmUtils/fdm_Misc.h>
#include <fdmUtils/fdm_Units.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

F16C_FLCS::F16C_FLCS() :
    m_ailerons_max ( 0.0 ),
    m_elevator_max ( 0.0 ),
    m_rudder_max   ( 0.0 ),
    m_flaps_le_max ( 0.0 ),

    m_ailerons      ( 0.0 ),
    m_ailerons_norm ( 0.0 ),
    m_elevator      ( 0.0 ),
    m_elevator_norm ( 0.0 ),
    m_rudder        ( 0.0 ),
    m_rudder_norm   ( 0.0 ),
    m_flaps_le      ( 0.0 ),
    m_flaps_le_norm ( 0.0 ),

    m_timeStep ( 0.0 ),

    m_cat   ( CAT_I  ),
    m_gains ( Cruise ),

    m_alpha_deg ( 0.0 ),
    m_alpha_lag ( 0.0 ),
    m_alpha_dif ( 0.0 ),

    m_aoa_lef_1 ( 0.0 ),
    m_aoa_lef_2 ( 0.0 ),

    m_omg_q_lag  ( 0.0 ),
    m_omg_q_dif  ( 0.0 ),
    m_stick_lon  ( 0.0 ),
    m_g_response ( 0.0 ),
    m_g_command  ( 0.0 ),
    m_sca        ( 0.0 ),
    m_sca_1_dif  ( 0.0 ),
    m_sca_1_lag  ( 0.0 ),
    m_sca_2_lag  ( 0.0 ),
    m_pitch_int  ( 0.0 ),
    m_pitch_nfl  ( 0.0 ),
    m_elevator_1 ( 0.0 ),
    m_elevator_2 ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

F16C_FLCS::~F16C_FLCS() {}

////////////////////////////////////////////////////////////////////////////////

void F16C_FLCS::update( double timeStep, double angleOfAttack, double g_z,
                        double rollRate, double pitchRate,
                        double stickLat, double trimLat,
                        double stickLon, double trimLon,
                        double statPress, double dynPress )
{
    m_timeStep = timeStep;

    double alpha_deg_new = Units::rad2deg( angleOfAttack );
    m_alpha_dif = ( m_timeStep > 0.0 ) ? ( alpha_deg_new - m_alpha_deg ) / m_timeStep : 0.0;
    m_alpha_deg = alpha_deg_new;
    m_alpha_lag = Inertia::update( m_alpha_deg, m_alpha_lag, m_timeStep, 0.1 );

    // TODO: Aileron Rudder Interconnect (ARI), rudder authority limiter, yaw rate limiter

    // TODO (GR1F-16CJ-1, p.1-127)
//    if ( ( Units::mps2kts( m_aircraft->getAirspeed() ) < 400.0 && ( alt_flaps_extend || air_refuel_open ) )
//      || landing_gear_dn )
//    {
//        m_gains = Takeoff;
//    }
//    else
//    {
//        m_gains = Cruise;
//    }

    updateLEF( statPress, dynPress );
    updatePitch( pitchRate, g_z, stickLon, trimLon, dynPress ); // before roll
    updateRoll( rollRate, stickLat, trimLat, dynPress );
    updateYaw();
}

////////////////////////////////////////////////////////////////////////////////

void F16C_FLCS::updateLEF( double statPress, double dynPress )
{
    m_aoa_lef_1 = Inertia::update( 2.0 * m_alpha_dif / 7.25, m_aoa_lef_1, m_timeStep, 1.0 / 7.25 );
    m_aoa_lef_2 = Inertia::update( m_alpha_deg, m_aoa_lef_2, m_timeStep, 1.0 / 7.25 );

    // (NASA-TP-1538, p.34)
    // d_lef = 1.38 (2s+7.25)/(s+7.25) AoA - 9.05 q/p_s + 1.45
    double flaps_le_deg = 1.38 * ( m_aoa_lef_1 + m_aoa_lef_2 )
            - 9.05 * dynPress / statPress + 1.45;

    m_flaps_le = Misc::satur( 0.0, m_flaps_le_max, Units::deg2rad( flaps_le_deg ) );
    m_flaps_le_norm = m_flaps_le / m_flaps_le_max;
}

////////////////////////////////////////////////////////////////////////////////

void F16C_FLCS::updatePitch( double pitchRate, double g_z,
                             double stickLon, double trimLon,
                             double dynPress )
{
    double omg_q_deg = Units::rad2deg( pitchRate );
    double omg_q_lag_new = Inertia::update( omg_q_deg, m_omg_q_lag, m_timeStep, 1.0 );
    m_omg_q_dif = ( m_timeStep > 0.0 ) ? ( omg_q_lag_new - m_omg_q_lag ) / m_timeStep : 0.0;
    m_omg_q_lag = omg_q_lag_new;
    m_stick_lon = Inertia::update( stickLon, m_stick_lon, m_timeStep, 1.0/60.0 );
    m_g_response = Inertia::update( g_z - 1.0, m_g_response, m_timeStep, 0.02 );

    double q_dif_gained = 0.7 * getPitchRateGain( dynPress ) * m_omg_q_dif;

    double g_command_new = getGCommand( m_stick_lon, trimLon, dynPress, q_dif_gained );
    m_g_command = Inertia::update( g_command_new, m_g_command, m_timeStep, 1.0/8.3 );

    // stability/command augumentation (NASA-TP-1538, p.210)
    double sca_new = std::max( 0.0, 0.322 * ( q_dif_gained + m_alpha_lag - 15.0 ) )
            + 0.334 * m_omg_q_dif + m_g_response;

    m_sca_1_dif = ( m_timeStep > 0.0 ) ? ( sca_new - m_sca ) / m_timeStep : 0.0;
    m_sca = sca_new;
    m_sca_1_lag = Inertia::update( 0.25 * m_sca_1_dif, m_sca_1_lag, m_timeStep, 1.0 / 12.0 );
    m_sca_2_lag = Inertia::update( m_sca, m_sca_2_lag, m_timeStep, 1.0 / 12.0 );

    // (NASA-TP-1538, p.210)
    double g_command = 1.5 * getPitchLoopGain( dynPress ) * ( m_g_command + m_sca_1_lag + m_sca_2_lag );

    double elevator_max_deg = Units::rad2deg( m_elevator_max );

    // (NASA-TP-1538, p.210)
    // (TO 1F-16A-1, p.1-124)
    m_pitch_int = m_pitch_int + 5.0 * ( g_command - m_pitch_nfl ) * m_timeStep;
    double selector_input = m_pitch_int + g_command + 0.5 * m_alpha_lag;
    m_pitch_nfl = 5.0 * Misc::deadband( -elevator_max_deg, elevator_max_deg, selector_input );

    double elevator_1 = Inertia::update( selector_input + 0.0, m_elevator_1, m_timeStep, 1.0 / 20.2 );
    double elevator_2 = Inertia::update( selector_input - 0.0, m_elevator_2, m_timeStep, 1.0 / 20.2 );

    // (NASA-TP-1538, p.210)
    double elevator_delta_max = 60.0 * m_timeStep;
    m_elevator_1 = getElevatorMaxRate( m_elevator_1, elevator_1, elevator_delta_max );
    m_elevator_2 = getElevatorMaxRate( m_elevator_2, elevator_2, elevator_delta_max );

    m_elevator_1 = Misc::satur( -elevator_max_deg, elevator_max_deg, m_elevator_1 );
    m_elevator_2 = Misc::satur( -elevator_max_deg, elevator_max_deg, m_elevator_2 );

    double delta_h = 0.5 * ( m_elevator_1 + m_elevator_2 );
    double delta_d = 0.5 * ( m_elevator_1 - m_elevator_2 );

    m_elevator = Units::deg2rad( delta_h );
    m_elevator_norm = m_elevator / m_elevator_max;
}

////////////////////////////////////////////////////////////////////////////////

void F16C_FLCS::updateRoll( double rollRate, double stickLat, double trimLat, double dynPress )
{
    // TODO according to scheme
    double p_com = getRollRateCommand( stickLat, trimLat, dynPress, Units::rad2deg( m_elevator ) );
    double delta_p = p_com - rollRate;
    double ailerons_deg = -0.12 * Units::rad2deg( delta_p );
    double ailerons_rad = Units::deg2rad( ailerons_deg );

    m_ailerons = Misc::satur( -m_ailerons_max, m_ailerons_max, ailerons_rad );
    m_ailerons_norm = m_ailerons / m_ailerons_max;
}

////////////////////////////////////////////////////////////////////////////////

void F16C_FLCS::updateYaw()
{

}

////////////////////////////////////////////////////////////////////////////////

double F16C_FLCS::getElevatorMaxRate( double elevator_old, double elevator_new,
                                      double delta_max )
{
    if ( fabs( elevator_new - elevator_old ) < delta_max )
    {
        return elevator_new;
    }
    else
    {
        return elevator_old + Misc::sign( elevator_new - elevator_old ) * delta_max;
    }
}

////////////////////////////////////////////////////////////////////////////////

double F16C_FLCS::getGCommand( double stickLon, double trimLon, double dynPress,
                               double q_dif_gained )
{
    // negative g limit (NASA-TP-1538, p.211 - 62b)
    double g_min = -4.0;
    if ( dynPress < 1628.0 ) // 34 psf
    {
        g_min = -1.0;
    }
    else if ( dynPress < 8810.0 ) // 184 psf
    {
        g_min = -1.0 - ( 3.0 / ( 8810.0 - 1628.0 ) ) * ( dynPress - 1628.0 );
    }

    double g_max = getMaxG( q_dif_gained );

    // (NASA-TP-1538, p.210)
    double g_com = 9.0 * stickLon + 2.4 * trimLon;

    return Misc::satur( -g_max, -g_min, g_com );
}

////////////////////////////////////////////////////////////////////////////////

double F16C_FLCS::getMaxG( double q_dif_gained )
{
    // AoA/G Limiter (NASA-TP-1538, p.34, Droste, p.94, GR1F-16CJ-1, p.1-125)
    double g_max = 9.0;

    if ( m_cat == CAT_I )
    {
        double angle = m_alpha_lag + q_dif_gained;

        if ( angle >= 25.2 )
        {
            g_max = 0.0;
        }
        else if ( angle >= 20.4 )
        {
            g_max = 7.2612 - ( angle - 20.4 ) * 1.322;
        }
        else if ( angle >= 15.0 )
        {
            g_max = 9.0 - ( angle - 15.0 ) * 0.322;
        }
    }
    else
    {
        // TODO
    }

    return g_max;
}

////////////////////////////////////////////////////////////////////////////////

double F16C_FLCS::getPitchRateGain( double dynPress )
{
    double gain = 1.0;

    // (NASA-TP-1538, p.211 - 62c)
    if ( dynPress > 15280.0 )
    {
        gain = 0.32;
    }
    else if ( dynPress > 5250.0 )
    {
        gain = 1.0 - ( 1.0 - 0.32 ) / ( 15280.0 - 5250.0 ) * ( dynPress - 5250.0 );
    }

//    // (Marchand, p.20 - F3)
//    gain = getPitchLoopGain( dynPress );

    return gain;
}

////////////////////////////////////////////////////////////////////////////////

double F16C_FLCS::getPitchLoopGain( double dynPress )
{
    double gain = 1.0;

    // (NASA-TP-1538, p.211 - 62d)
    if ( dynPress > 43200.0 )
    {
        gain = 0.44;
    }
    else if ( dynPress > 12500.0 )
    {
        gain = 1.0 - ( 1.0 - 0.44 ) / ( 43200.0 - 12500.0 ) * ( dynPress - 12500.0 );
    }

//    // (Marchand, p.20 - F3)
//    if ( dynPress > 143640.0 ) // 3,000 psf
//    {
//        gain = 0.083;
//    }
//    else if ( dynPress > 38304.0 ) // 800 psf
//    {
//        gain = 0.533 - ( 0.533 - 0.083 ) / ( 143640.0 - 38304.0 ) * ( dynPress - 38304.0 );
//    }
//    else if ( dynPress > 14364.0 ) // 300 psf
//    {
//        gain = 1.0 - ( 1.0 - 0.533 ) / ( 38304.0 - 14364.0 ) * ( dynPress - 14364.0 );
//    }

    return gain;
}

////////////////////////////////////////////////////////////////////////////////

double F16C_FLCS::getRollRateCommand( double stickLat, double trimLat, double dynPress,
                                      double delta_h )
{
    // (NASA-TP-1538, p.214)
    double p_com_pilot = -308.0 * stickLat - 40.0 * trimLat * 1.67;

    // (NASA-TP-1538, p.129, Droste, p.99, GR1F-16CJ-1, p.1-125)
    double delta_p_max = -0.0115 * std::min( dynPress - 10500.0, 0.0 )
            + 4.0 * std::max( m_alpha_deg - 15.0, 0.0 )
            + 4.0 * std::max( delta_h     -  5.0, 0.0 );
            // TODO Total rudder command (from pilot and FLCS) exceeding 20deg (GR1F-16CJ-1, p.1-125)
            // TODO Combination of horizontal tail greater than 15deg trailing edge down and AoA above 22deg (GR1F-16CJ-1, p.1-125)

    double p_max = 308.0 - std::min( delta_p_max, 228.0 );

    if ( m_cat == CAT_III )
    {
        p_max *= 0.6;
    }

    return Units::deg2rad( Misc::satur( -p_max, p_max, p_com_pilot ) );
}
