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

#include <iostream>
#include <algorithm>

#include <fdmSys/fdm_Lag.h>
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

    m_ailerons_max_deg ( 0.0 ),
    m_elevator_max_deg ( 0.0 ),
    m_rudder_max_deg   ( 0.0 ),
    m_flaps_le_max_deg ( 0.0 ),

    m_ailerons      ( 0.0 ),
    m_ailerons_norm ( 0.0 ),
    m_elevator      ( 0.0 ),
    m_elevator_norm ( 0.0 ),
    m_rudder        ( 0.0 ),
    m_rudder_norm   ( 0.0 ),
    m_flaps_le      ( 0.0 ),
    m_flaps_le_norm ( 0.0 ),
    m_flaps_te      ( 0.0 ),
    m_flaps_te_norm ( 0.0 ),

    m_timeStep ( 0.0 ),

    m_cat   ( CAT_I  ),
    m_gains ( Cruise ),

    m_alpha_lef ( 0 ),

    m_delta_fl_lag ( 0 ),
    m_delta_fr_lag ( 0 ),
    m_delta_fl_com ( 0.0 ),
    m_delta_fr_com ( 0.0 ),
    m_delta_fl     ( 0.0 ),
    m_delta_fr     ( 0.0 ),

    m_stick_lat   ( 0 ),
    m_p_com_lag   ( 0 ),
    m_p_com_pos   ( 0 ),
    m_p_com_neg   ( 0 ),
    m_omg_p_lag   ( 0 ),
    m_omg_p_fil   ( 0 ),
    m_delta_a_lag ( 0 ),
    m_delta_ac  ( 0.0 ),
    m_delta_dc  ( 0.0 ),
    m_delta_a   ( 0.0 ),

    m_stick_lon  ( 0 ),
    m_alpha_lag  ( 0 ),
    m_g_com_lag  ( 0 ),
    m_omg_q_lag  ( 0 ),
    m_omg_q_fil  ( 0 ),
    m_g_z_input  ( 0 ),
    m_sca_bias_1 ( 0 ),
    m_sca_bias_2 ( 0 ),
    m_sca_bias_3 ( 0 ),
    m_u_sca_fil  ( 0 ),
    m_actuator_1 ( 0 ),
    m_actuator_2 ( 0 ),
    m_pitch_int  ( 0.0 ),
    m_pitch_nfl  ( 0.0 ),
    m_elevator_1 ( 0.0 ),
    m_elevator_2 ( 0.0 ),
    m_delta_h    ( 0.0 ),
    m_delta_d    ( 0.0 ),

    m_pedals      ( 0 ),
    m_omg_r_lag   ( 0 ),
    m_omg_p_yaw   ( 0 ),
    m_u_sum_ll1   ( 0 ),
    m_u_sum_ll2   ( 0 ),
    m_delta_r_lag ( 0 ),
    m_delta_r   ( 0.0 )
{
    // lef
    m_alpha_lef = new LeadLag( 2.0, 7.25, 1.0, 7.25 );

    // tef
    m_delta_fl_lag = new Lag( 1.0 / 20.0 );
    m_delta_fr_lag = new Lag( 1.0 / 20.0 );

    // lat
    m_stick_lat   = new Lag( 1.0 / 60.0 );
    m_p_com_lag   = new Lag( 1.0 / 10.0 );
    m_p_com_pos   = new LeadLag( 6.0, 0.0, 1.0, 20.0 );
    m_p_com_neg   = new LeadLag( 6.0, 0.0, 1.0, 20.0 );
    m_omg_p_lag   = new Lag( 1.0 / 50.0 );
    m_omg_p_fil   = new Filter2( 4.0, 64.0, 6400.0, 1.0, 80.0, 6400.0 );
    m_delta_a_lag = new Lag( 1.0 / 20.0 );

    // lon
    m_stick_lon  = new Lag( 1.0 / 60.0 );
    m_alpha_lag  = new Lag( 1.0 / 10.0 );
    m_g_com_lag  = new Lag( 1.0 / 8.3 );
    m_omg_q_lag  = new Lag( 1.0 / 50.0 );
    m_omg_q_fil  = new LeadLag( 1.0, 0.0, 1.0, 1.0 );
    m_g_z_input  = new Lag( 1.0 / 50.0 );
    m_sca_bias_1 = new Lag( 1.0 / 10.0 );
    m_sca_bias_2 = new Lag( 1.0 / 0.125 );
    m_sca_bias_3 = new Lag( 1.0 / 0.125 );
    m_u_sca_fil  = new LeadLag( 3.0, 12.0, 1.0, 12.0 );
    m_actuator_1 = new Lag( 1.0 / 20.0 );
    m_actuator_2 = new Lag( 1.0 / 20.0 );

    // yaw
    m_pedals      = new Lag( 1.0 / 60.0 );
    m_omg_r_lag   = new Lag( 1.0 / 50.0 );
    m_omg_p_yaw   = new Filter2( 1.0, 0.0, 3025.0, 1.0, 110.0, 3025.0 );
    m_u_sum_ll1   = new LeadLag( 3.0, 15.0, 1.0, 15.0 );
    m_u_sum_ll2   = new LeadLag( 1.5,  0.0, 1.0,  1.0 );
    m_delta_r_lag = new Lag( 1.0 / 20.0 );
}

////////////////////////////////////////////////////////////////////////////////

F16C_FLCS::~F16C_FLCS()
{
    // lef
    if ( m_alpha_lef ) delete m_alpha_lef;
    m_alpha_lef = 0;

    // tef
    if ( m_delta_fl_lag ) delete m_delta_fl_lag;
    m_delta_fl_lag = 0;

    if ( m_delta_fr_lag ) delete m_delta_fr_lag;
    m_delta_fr_lag = 0;

    // lat
    if ( m_stick_lat ) delete m_stick_lat;
    m_stick_lat = 0;

    if ( m_p_com_lag ) delete m_p_com_lag;
    m_p_com_lag = 0;

    if ( m_p_com_pos ) delete m_p_com_pos;
    m_p_com_pos = 0;

    if ( m_p_com_neg ) delete m_p_com_neg;
    m_p_com_neg = 0;

    if ( m_omg_p_lag ) delete m_omg_p_lag;
    m_omg_p_lag = 0;

    if ( m_omg_p_fil ) delete m_omg_p_fil;
    m_omg_p_fil = 0;

    if ( m_delta_a_lag ) delete m_delta_a_lag;
    m_delta_a_lag = 0;

    // lon
    if ( m_stick_lon ) delete m_stick_lon;
    m_stick_lon = 0;

    if ( m_alpha_lag ) delete m_alpha_lag;
    m_alpha_lag = 0;

    if ( m_g_com_lag ) delete m_g_com_lag;
    m_g_com_lag = 0;

    if ( m_omg_q_lag ) delete m_omg_q_lag;
    m_omg_q_lag = 0;

    if ( m_omg_q_fil ) delete m_omg_q_fil;
    m_omg_q_fil = 0;

    if ( m_g_z_input ) delete m_g_z_input;
    m_g_z_input = 0;

    if ( m_sca_bias_1 ) delete m_sca_bias_1;
    m_sca_bias_1 = 0;

    if ( m_sca_bias_2 ) delete m_sca_bias_2;
    m_sca_bias_2 = 0;

    if ( m_sca_bias_3 ) delete m_sca_bias_3;
    m_sca_bias_3 = 0;

    if ( m_u_sca_fil ) delete m_u_sca_fil;
    m_u_sca_fil = 0;

    if ( m_actuator_1 ) delete m_actuator_1;
    m_actuator_1 = 0;

    if ( m_actuator_2 ) delete m_actuator_2;
    m_actuator_2 = 0;

    // yaw
    if ( m_pedals ) delete m_pedals;
    m_pedals = 0;

    if ( m_omg_r_lag ) delete m_omg_r_lag;
    m_omg_r_lag = 0;

    if ( m_omg_p_yaw ) delete m_omg_p_yaw;
    m_omg_p_yaw = 0;

    if ( m_u_sum_ll1 ) delete m_u_sum_ll1;
    m_u_sum_ll1 = 0;

    if ( m_u_sum_ll2 ) delete m_u_sum_ll2;
    m_u_sum_ll2 = 0;

    if ( m_delta_r_lag ) delete m_delta_r_lag;
    m_delta_r_lag = 0;
}

////////////////////////////////////////////////////////////////////////////////

void F16C_FLCS::update( double timeStep, double angleOfAttack,
                        double g_y, double g_z,
                        double rollRate, double pitchRate, double yawRate,
                        double ctrlLat, double trimLat,
                        double ctrlLon, double trimLon,
                        double ctrlYaw, double trimYaw,
                        double statPress, double dynPress,
                        bool alt_flaps_ext, bool refuel_door_open,
                        bool lg_handle_dn )
{
    if ( timeStep > 0.0 )
    {
        m_timeStep = timeStep;

        // (Marchand, p.20, Note A)
        // 646 psf = 30,930.6473 Pa
        if ( ( dynPress < 30930.6473 && refuel_door_open ) || lg_handle_dn || alt_flaps_ext )
        {
            m_gains = Landing;
        }
        else
        {
            m_gains = Cruise;
        }

        // stanby gains
        if ( 0 )
        {
            // (Marchand, p.20)
            //   200 psf =   9,576.0518 Pa
            // 1,400 psf =  67,032.3626 Pa
            // 2,116 psf = 101,314.6280 Pa
            dynPress = ( m_gains == Landing ) ? 9576.0518 : 67032.3626;
            statPress = 101314.628;
        }

        updateTEF( alt_flaps_ext, lg_handle_dn );
        updateLEF( angleOfAttack, statPress, dynPress );
        updateLat( ctrlLat, trimLat, rollRate );
        updateLon( ctrlLon, trimLon, pitchRate, angleOfAttack, dynPress, g_z );
        updateYaw( ctrlYaw, trimYaw, yawRate, statPress, dynPress, g_y );
    }
}

////////////////////////////////////////////////////////////////////////////////

void F16C_FLCS::setAilerons_max( double ailerons_max )
{
    m_ailerons_max = ailerons_max;
    m_ailerons_max_deg = Units::rad2deg( m_ailerons_max );
}

////////////////////////////////////////////////////////////////////////////////

void F16C_FLCS::setElevator_max( double elevator_max )
{
    m_elevator_max = elevator_max;
    m_elevator_max_deg = Units::rad2deg( m_elevator_max );
}

////////////////////////////////////////////////////////////////////////////////

void F16C_FLCS::setRudder_max( double rudder_max )
{
    m_rudder_max = rudder_max;
    m_rudder_max_deg = Units::rad2deg( m_rudder_max );
}

////////////////////////////////////////////////////////////////////////////////

void F16C_FLCS::setFlaps_le_max( double flaps_le_max )
{
    m_flaps_le_max = flaps_le_max;
    m_flaps_le_max_deg = Units::rad2deg( m_flaps_le_max );
}

////////////////////////////////////////////////////////////////////////////////

void F16C_FLCS::updateLEF( double angleOfAttack, double statPress, double dynPress )
{
    m_alpha_lef->update( Units::rad2deg( angleOfAttack ), m_timeStep );

    // (NASA-TP-1538, p.34)
    // d_lef = 1.38 (2s+7.25)/(s+7.25) AoA - 9.05 q/p_s + 1.45
    double flaps_le_deg = 1.38 * m_alpha_lef->getValue()
            - 9.05 * dynPress / statPress + 1.45;

    m_flaps_le = Misc::satur( 0.0, m_flaps_le_max, Units::deg2rad( flaps_le_deg ) );
    m_flaps_le_norm = m_flaps_le / m_flaps_le_max;
}

////////////////////////////////////////////////////////////////////////////////

void F16C_FLCS::updateTEF( bool alt_flaps_ext, bool lg_handle_dn )
{
    // (Marchand, p.20)
    double flaps_com = ( alt_flaps_ext || lg_handle_dn ) ? 20.0 : 0.0;

    // TODO flaps_com

    double limit_fl = std::max( 0.0, m_delta_fl_com - 21.5 );
    double limit_fr = std::max( 0.0, m_delta_fr_com - 21.5 );

    m_delta_fl_com = flaps_com - m_delta_ac - limit_fr;
    m_delta_fr_com = flaps_com + m_delta_ac - limit_fl;

    double flaperons_max_deg = Units::rad2deg( m_ailerons_max );
    double delta_fl_com = Misc::satur( -flaperons_max_deg, flaperons_max_deg, m_delta_fl_com );
    double delta_fr_com = Misc::satur( -flaperons_max_deg, flaperons_max_deg, m_delta_fr_com );

    m_delta_fl_lag->update( delta_fl_com, m_timeStep );
    m_delta_fr_lag->update( delta_fr_com, m_timeStep );

    double flaperons_delta_max = 80.0 * m_timeStep;
    m_delta_fl = getSurfaceMaxRate( m_delta_fl, m_delta_fl_lag->getValue(), flaperons_delta_max );
    m_delta_fr = getSurfaceMaxRate( m_delta_fr, m_delta_fr_lag->getValue(), flaperons_delta_max );

    double flaperon_l = Misc::satur( -m_ailerons_max, m_ailerons_max, Units::deg2rad( m_delta_fl ) );
    double flaperon_r = Misc::satur( -m_ailerons_max, m_ailerons_max, Units::deg2rad( m_delta_fr ) );

    double flaps_te_r = flaperon_r - m_ailerons;
    double flaps_te_l = flaperon_l + m_ailerons;

    m_flaps_te_norm = 0.5 * ( flaps_te_r + flaps_te_l ) / m_ailerons_max;
    m_flaps_te = m_flaps_te_norm * m_ailerons_max - Units::deg2rad( 1.5 );
}

////////////////////////////////////////////////////////////////////////////////

void F16C_FLCS::updateLat( double ctrlLat, double trimLat,
                           double rollRate )
{
    double omg_p_deg = Units::rad2deg( rollRate );

    m_stick_lat->update( ctrlLat, m_timeStep );

    // (NASA-TP-1538, p.214)
    double p_com = m_stick_lat->getValue();

    // (Marchand, p.20 - FIG. C)
    if ( m_gains == Landing ) p_com *= 0.542;

    double p_loop_pos = std::max( 0.0, m_p_com_pos->getValue() );
    double p_loop_neg = std::min( 0.0, m_p_com_neg->getValue() );
    m_p_com_lag->update( p_com - p_loop_pos - p_loop_neg, m_timeStep );

    m_p_com_pos->update( std::max( 0.0, m_p_com_lag->getValue() ), m_timeStep );
    m_p_com_neg->update( std::min( 0.0, m_p_com_lag->getValue() ), m_timeStep );

    m_omg_p_lag->update( omg_p_deg, m_timeStep );
    m_omg_p_fil->update( m_omg_p_lag->getValue(), m_timeStep );

    double roll_control = m_omg_p_fil->getValue() - ( m_p_com_lag->getValue() + trimLat * 1.67 );

    if ( m_alpha_lag->getValue() > 29.0 )
    {
        roll_control = 8.34 * m_omg_r_lag->getValue();
    }

    m_delta_ac = Misc::satur( -m_ailerons_max_deg, m_ailerons_max_deg, 0.12 * roll_control );
    m_delta_dc = 0.25 * m_delta_ac;

    m_delta_a_lag->update( m_delta_ac, m_timeStep );

    double ailerons_delta_max = 80.0 * m_timeStep;
    m_delta_a = getSurfaceMaxRate( m_delta_a, m_delta_a_lag->getValue(), ailerons_delta_max );

    m_ailerons = Misc::satur( -m_ailerons_max, m_ailerons_max, Units::deg2rad( m_delta_a ) );
    m_ailerons_norm = m_ailerons / m_ailerons_max;
}

////////////////////////////////////////////////////////////////////////////////

void F16C_FLCS::updateLon( double ctrlLon, double trimLon,
                           double pitchRate,
                           double angleOfAttack, double dynPress, double g_z )
{
    double alpha_deg = Units::rad2deg( angleOfAttack );
    double omg_q_deg = Units::rad2deg( pitchRate );

    m_alpha_lag->update( Misc::satur( -5.0, 30.0, alpha_deg ), m_timeStep );
    m_stick_lon->update( ctrlLon   , m_timeStep );
    m_g_z_input->update( g_z - 1.0 , m_timeStep );
    m_omg_q_lag->update( omg_q_deg , m_timeStep );
    m_omg_q_fil->update( m_omg_q_lag->getValue(), m_timeStep );

    double g_com = m_stick_lon->getValue() + trimLon;
    double g_max =  8.0;
    double g_min = -4.0;

    // negative g limit
    // (NASA-TP-1538, p.211 - 62b)
    if ( m_gains == Cruise )
    {
        if ( dynPress < 1628.0 ) // 34 psf
        {
            g_min = -1.0;
        }
        else if ( dynPress < 8810.0 ) // 184 psf
        {
            g_min = -1.0 - ( 3.0 / ( 8810.0 - 1628.0 ) ) * ( dynPress - 1628.0 );
        }
    }

    m_g_com_lag->update( Misc::satur( g_min, g_max, g_com ), m_timeStep );

    double q_gained = 0.7 * getGainPitchRate( dynPress ) * m_omg_q_fil->getValue();
    double aoa_limit = std::max( 0.0, m_alpha_lag->getValue() - 20.4 + q_gained );

    double g_command = aoa_limit - m_g_com_lag->getValue();

    // stability/command augumentation
    double aoa_bias = 15.0;
    //std::cout << __FILE__ << "(" << __LINE__ << ") " << aoa_bias << std::endl;

    double u_sca = std::max( 0.0, 0.322 * ( q_gained + m_alpha_lag->getValue() - aoa_bias ) )
            + 0.334 * m_omg_q_fil->getValue() + m_g_z_input->getValue();

    m_u_sca_fil->update( u_sca, m_timeStep );

    double pitch_int_input = 1.5 * getGainPitchLoop( dynPress ) * ( g_command + m_u_sca_fil->getValue() );

    m_pitch_int = m_pitch_int + 5.0 * ( pitch_int_input - m_pitch_nfl ) * m_timeStep;
    double selector_input = m_pitch_int + pitch_int_input + 0.5 * m_alpha_lag->getValue();
    m_pitch_nfl = 5.0 * Misc::deadband( -m_elevator_max_deg, m_elevator_max_deg, selector_input );

    m_actuator_1->update( selector_input + m_delta_dc, m_timeStep );
    m_actuator_2->update( selector_input - m_delta_dc, m_timeStep );

    double elevator_delta_max = 60.0 * m_timeStep;
    m_elevator_1 = getSurfaceMaxRate( m_elevator_1, m_actuator_1->getValue(), elevator_delta_max );
    m_elevator_2 = getSurfaceMaxRate( m_elevator_2, m_actuator_2->getValue(), elevator_delta_max );

    m_elevator_1 = Misc::satur( -m_elevator_max_deg, m_elevator_max_deg, m_elevator_1 );
    m_elevator_2 = Misc::satur( -m_elevator_max_deg, m_elevator_max_deg, m_elevator_2 );

    m_delta_h = 0.5 * ( m_elevator_1 + m_elevator_2 );
    m_delta_d = 0.5 * ( m_elevator_1 - m_elevator_2 );

    m_elevator = Units::deg2rad( m_delta_h );
    m_elevator_norm = m_elevator / m_elevator_max;

    m_elevons = Units::deg2rad( m_delta_d );
}

////////////////////////////////////////////////////////////////////////////////

void F16C_FLCS::updateYaw( double ctrlYaw, double trimYaw,
                           double yawRate,
                           double statPress, double dynPress, double g_y )
{
    double omg_r_deg = Units::rad2deg( yawRate  );

    m_pedals->update( ctrlYaw, m_timeStep );
    m_omg_r_lag->update( omg_r_deg, m_timeStep );
    m_omg_p_yaw->update( m_omg_p_lag->getValue(), m_timeStep );

    double alpha_gain = 1.0;

    if ( m_alpha_lag->getValue() > 30.0 )
    {
        alpha_gain = 0.0;
    }
    else if ( m_alpha_lag->getValue() > 20.0 )
    {
        alpha_gain = 1.0 - ( m_alpha_lag->getValue() - 20.0 ) / ( 30.0 - 20.0 );
    }

    double r_com = alpha_gain * ( m_pedals->getValue() + trimYaw );
    double u_sum = m_omg_r_lag->getValue() - ( 1.0 / 57.3 ) * m_omg_p_yaw->getValue() * m_alpha_lag->getValue();

    m_u_sum_ll1->update( u_sum, m_timeStep );
    m_u_sum_ll2->update( m_u_sum_ll1->getValue(), m_timeStep );

    // Aileron Rudder Interconnect (ARI)
    double ari = 0.075 * m_alpha_lag->getValue() - m_alpha_lag->getValue() * getGainARI( statPress, dynPress );

    double r_auto = 0.5 * ( m_u_sum_ll2->getValue() - 19.32 * g_y ) + m_delta_ac * ari;

    if ( m_alpha_lag->getValue() > 29.0 )
    {
        r_auto = 0.75 * m_omg_r_lag->getValue();
    }

    double delta_rc = r_com + r_auto;

    m_delta_r_lag->update( delta_rc, m_timeStep );

    double rudder_delta_max = 120.0 * m_timeStep;
    m_delta_r = getSurfaceMaxRate( m_delta_r, m_delta_r_lag->getValue(), rudder_delta_max );
    m_delta_r = Misc::satur( -m_rudder_max_deg, m_rudder_max_deg, m_delta_r );

    m_rudder = Units::deg2rad( m_delta_r );
    m_rudder_norm = m_rudder / m_rudder_max;
}

////////////////////////////////////////////////////////////////////////////////

double F16C_FLCS::getGainARI( double statPress, double dynPress )
{
    double gain_1 = 0.0;
    double gain_2 = 1.0;

    // (NASA-TP-1538, p.216)
    if ( -10.0 < m_alpha_lag->getValue() && m_alpha_lag->getValue() < 10.0 )
    {
        gain_1 = 1.0 - fabs( m_alpha_lag->getValue() ) / 10.0;
    }

    double q_p = dynPress / statPress;

    // (NASA-TP-1538, p.216)
    if ( q_p <= 0.187 )
    {
        gain_2 = 0.0;
    }
    else if ( 0.187 <= q_p && q_p < 1.129 )
    {
        gain_2 = ( q_p - 0.187 ) / ( 1.129 - 0.187 );
    }
    else
    {
        gain_2 = 1.0;
    }

    return gain_1 * gain_2;
}

////////////////////////////////////////////////////////////////////////////////

double F16C_FLCS::getGainPitchRate( double dynPress )
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

    return gain;
}

////////////////////////////////////////////////////////////////////////////////

double F16C_FLCS::getGainPitchLoop( double dynPress )
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

    return gain;
}

////////////////////////////////////////////////////////////////////////////////

double F16C_FLCS::getSurfaceMaxRate( double d_old, double d_new, double delta_max )
{
    if ( fabs( d_new - d_old ) < delta_max )
    {
        return d_new;
    }
    else
    {
        return d_old + Misc::sign( d_new - d_old ) * delta_max;
    }
}
