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
    m_elevons       ( 0.0 ),
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

    m_flaps_int ( 0.0 ),
    m_flaps_com ( 0.0 ),

    m_stick_lat    ( 0 ),
    m_p_com_lag    ( 0 ),
    m_p_com_pos    ( 0 ),
    m_p_com_neg    ( 0 ),
    m_omg_p_lag    ( 0 ),
    m_omg_p_fil    ( 0 ),
    m_delta_fl_lag ( 0 ),
    m_delta_fr_lag ( 0 ),

    m_delta_flc ( 0.0 ),
    m_delta_frc ( 0.0 ),
    m_delta_fl  ( 0.0 ),
    m_delta_fr  ( 0.0 ),
    m_delta_ac  ( 0.0 ),
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
    m_u_sca_fil2 ( 0 ),
    m_actuator_l ( 0 ),
    m_actuator_r ( 0 ),

    m_pitch_int ( 0.0 ),
    m_delta_htl ( 0.0 ),
    m_delta_htr ( 0.0 ),
    m_delta_h   ( 0.0 ),
    m_delta_d   ( 0.0 ),

    m_pedals      ( 0 ),
    m_omg_r_lag   ( 0 ),
    m_omg_p_yaw   ( 0 ),
    m_u_sum_ll1   ( 0 ),
    m_u_sum_ll2   ( 0 ),
    m_delta_r_fil ( 0 ),
    m_delta_r_lag ( 0 ),

    m_delta_r   ( 0.0 ),

    m_gun_compensation ( 0.0 )
{
    // lef
    m_alpha_lef = new LeadLag( 2.0, 7.25, 1.0, 7.25 );

    // lat
    m_stick_lat    = new Lag( 1.0 / 60.0 );
    m_p_com_lag    = new Lag( 1.0 / 10.0 );
    m_p_com_pos    = new LeadLag( 6.0, 0.0, 1.0, 20.0 );
    m_p_com_neg    = new LeadLag( 6.0, 0.0, 1.0, 20.0 );
    m_omg_p_lag    = new Lag( 1.0 / 50.0 );
    m_omg_p_fil    = new Filter2( 4.0, 64.0, 6400.0, 1.0, 80.0, 6400.0 );
    m_delta_fl_lag = new Lag( 1.0 / 20.0 );
    m_delta_fr_lag = new Lag( 1.0 / 20.0 );

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
    m_u_sca_fil2 = new Filter2( 2.0, 20.0, 3500.0, 1.0, 40.0, 3500.0 );
    m_actuator_l = new Lag( 1.0 / 20.0 );
    m_actuator_r = new Lag( 1.0 / 20.0 );

    // yaw
    m_pedals      = new Lag( 1.0 / 60.0 );
    m_omg_r_lag   = new Lag( 1.0 / 50.0 );
    m_omg_p_yaw   = new Filter2( 1.0, 0.0, 3025.0, 1.0, 110.0, 3025.0 );
    m_u_sum_ll1   = new LeadLag( 3.0, 15.0, 1.0, 15.0 );
    m_u_sum_ll2   = new LeadLag( 1.5,  0.0, 1.0,  1.0 );
    m_delta_r_fil = new Filter2( 1.0, 0.0, 1225.0, 1.0, 70.0, 1225.0 );
    m_delta_r_lag = new Lag( 1.0 / 20.0 );
}

////////////////////////////////////////////////////////////////////////////////

F16C_FLCS::~F16C_FLCS()
{
    // lef
    if ( m_alpha_lef ) delete m_alpha_lef;
    m_alpha_lef = 0;

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

    if ( m_delta_fl_lag ) delete m_delta_fl_lag;
    m_delta_fl_lag = 0;

    if ( m_delta_fr_lag ) delete m_delta_fr_lag;
    m_delta_fr_lag = 0;

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

    if ( m_u_sca_fil2 ) delete m_u_sca_fil2;
    m_u_sca_fil2 = 0;

    if ( m_actuator_l ) delete m_actuator_l;
    m_actuator_l = 0;

    if ( m_actuator_r ) delete m_actuator_r;
    m_actuator_r = 0;

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

    if ( m_delta_r_fil ) delete m_delta_r_fil;
    m_delta_r_fil = 0;

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
                        bool lg_handle_dn, bool touchdown )
{
    if ( timeStep > 0.0 )
    {
        m_timeStep = timeStep;

        // (AD-A055-417, p.20, Note A)
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
            // (AD-A055-417, p.20)
            //   200 psf =   9,576.0518 Pa
            // 1,400 psf =  67,032.3626 Pa
            // 2,116 psf = 101,314.6280 Pa
            dynPress = ( m_gains == Landing ) ? 9576.0518 : 67032.3626;
            statPress = 101314.628;
        }

        double q_p = dynPress / statPress;

        updateTEF( q_p, alt_flaps_ext, lg_handle_dn );
        updateLEF( angleOfAttack, q_p );
        updateLat( ctrlLat, trimLat, rollRate );
        updateLon( ctrlLon, trimLon, pitchRate, angleOfAttack, q_p, dynPress, g_z, touchdown );
        updateYaw( ctrlYaw, trimYaw, yawRate, q_p, g_y );
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

void F16C_FLCS::updateLEF( double angleOfAttack, double q_p )
{
    m_alpha_lef->update( Units::rad2deg( angleOfAttack ), m_timeStep );

    // (NASA-TP-1538, p.34)
    // delta_lef = 1.38 (2s+7.25)/(s+7.25) alpha - 9.05 q/p_s + 1.45
    double flaps_le_deg = 1.38 * m_alpha_lef->getValue()
            - 9.05 * q_p + 1.45;

    m_flaps_le = Misc::satur( 0.0, m_flaps_le_max, Units::deg2rad( flaps_le_deg ) );
    m_flaps_le_norm = m_flaps_le / m_flaps_le_max;
}

////////////////////////////////////////////////////////////////////////////////

void F16C_FLCS::updateTEF( double q_p, bool alt_flaps_ext, bool lg_handle_dn )
{
    // (AD-A055-417, p.20)
    double flaps_com = ( alt_flaps_ext || lg_handle_dn ) ? 20.0 : 0.0;

    double flaps_trans = Misc::satur( -2.0, 2.0, getGainF9( q_p ) );
    double flaps_limit = Misc::satur( -0.625, 0.625, flaps_com + flaps_trans - m_flaps_int );
    m_flaps_int = m_flaps_int + 8.0 * m_timeStep * flaps_limit;
    m_flaps_com = m_flaps_int + 1.5;

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
    // (AD-A055-417, p.20)
    double omg_p_deg = Units::rad2deg( rollRate );

    m_stick_lat->update( ctrlLat, m_timeStep );

    double p_com = m_stick_lat->getValue();

    if ( m_gains == Landing ) p_com *= 0.542;

    double p_loop_pos = std::max( 0.0, m_p_com_pos->getValue() );
    double p_loop_neg = std::min( 0.0, m_p_com_neg->getValue() );
    m_p_com_lag->update( p_com - p_loop_pos - p_loop_neg, m_timeStep );

    m_p_com_pos->update( std::max( 0.0, m_p_com_lag->getValue() ), m_timeStep );
    m_p_com_neg->update( std::min( 0.0, m_p_com_lag->getValue() ), m_timeStep );

    m_omg_p_lag->update( omg_p_deg, m_timeStep );
    m_omg_p_fil->update( m_omg_p_lag->getValue(), m_timeStep );

    double roll_ap_tie_in = 0.0; // TODO
    double roll_control = m_omg_p_fil->getValue()
            - ( m_p_com_lag->getValue() + trimLat * 1.67 )
            - roll_ap_tie_in
            + 1.67 * m_gun_compensation;

    double ailerons_com = Misc::satur( -m_ailerons_max_deg, m_ailerons_max_deg, 0.12 * roll_control );

    double limit_fl = std::max( 0.0, m_delta_flc - 21.5 );
    double limit_fr = std::max( 0.0, m_delta_frc - 21.5 );

    double flaperons_max_deg = Units::rad2deg( m_ailerons_max );
    m_delta_flc = Misc::satur( -flaperons_max_deg, flaperons_max_deg, m_flaps_com - ailerons_com - limit_fr );
    m_delta_frc = Misc::satur( -flaperons_max_deg, flaperons_max_deg, m_flaps_com + ailerons_com - limit_fl );

    m_delta_ac = m_delta_frc - m_delta_flc;

    m_delta_fl_lag->update( m_delta_flc, m_timeStep );
    m_delta_fr_lag->update( m_delta_frc, m_timeStep );

    double flaperons_delta_max = 80.0 * m_timeStep;
    m_delta_fl = getSurfaceMaxRate( m_delta_fl, m_delta_fl_lag->getValue(), flaperons_delta_max );
    m_delta_fr = getSurfaceMaxRate( m_delta_fr, m_delta_fr_lag->getValue(), flaperons_delta_max );

    m_delta_a = 0.5 * ( m_delta_fr - m_delta_fl );

    m_ailerons = Misc::satur( -m_ailerons_max, m_ailerons_max, Units::deg2rad( m_delta_a ) );
    m_ailerons_norm = m_ailerons / m_ailerons_max;
}

////////////////////////////////////////////////////////////////////////////////

void F16C_FLCS::updateLon( double ctrlLon, double trimLon,
                           double pitchRate,
                           double angleOfAttack, double q_p, double dynPress, double g_z,
                           bool touchdown )
{
    // (AD-A055-417, p.20)
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

    // negative g limit (AD-A055-417, p.20 - F1)
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

    m_g_com_lag->update( Misc::satur( g_min, g_max, g_com ) * ( touchdown ? 1.0 : 0.5 ), m_timeStep );

    double q_gained = 0.7 * getGainF3( dynPress ) * m_omg_q_fil->getValue();
    double aoa_limit = std::max( 0.0, 0.5 * ( m_alpha_lag->getValue() - 20.4 + q_gained ) );

    double pitch_ap_tie_in = 0.0; // TODO
    double g_command = aoa_limit - m_g_com_lag->getValue() - pitch_ap_tie_in;

    m_sca_bias_1->update( touchdown ? 0.0 : 6.0, m_timeStep );
    m_sca_bias_2->update( m_gains == Landing ? 9.0 : 0.0, m_timeStep );
    m_sca_bias_3->update( m_gains == Landing ? 1.0 : 0.0, m_timeStep );

    double aoa_bias = 9.0 - m_sca_bias_2->getValue() + m_sca_bias_1->getValue();

    double u_sca_1 = 0.161 * ( q_gained + m_alpha_lag->getValue() - aoa_bias );
    double u_sca_2 = ( touchdown ? 0.231 : 0.167 ) * m_omg_q_fil->getValue()
            + 0.5 * m_g_z_input->getValue();

    double u_sca = std::max( 0.0, u_sca_1 ) + std::min( 0.0, u_sca_1 ) * m_sca_bias_3->getValue()
            + u_sca_2;

    m_u_sca_fil->update( u_sca, m_timeStep );
    m_u_sca_fil2->update( m_u_sca_fil->getValue(), m_timeStep );

    double pitch_gained = 3.0 * getGainF3( dynPress ) * ( g_command + m_u_sca_fil2->getValue() );
    double alpha_gained = getGainF2( q_p ) * m_alpha_lag->getValue();

    double k_a = 2000.0; // open loop amplifier gain (AD-A055-417, p.22)
    double pitch_out = m_pitch_int + pitch_gained;
    double pitch_nfl = 5.0 * Misc::deadband( -m_elevator_max_deg, m_elevator_max_deg, pitch_out + alpha_gained );
    double pitch_inp = ( touchdown ? -0.5 * m_pitch_int : pitch_gained - pitch_nfl )
            - k_a * Misc::deadband( -m_elevator_max_deg, m_elevator_max_deg, m_pitch_int );

    // pitch integrating
    m_pitch_int = m_pitch_int + 5.0 * pitch_inp * m_timeStep;

    double selector_input = m_pitch_int + pitch_gained + alpha_gained;

    double delta_dc = 0.5 * getGainF10( q_p ) * m_delta_ac;

    m_actuator_l->update( selector_input - delta_dc, m_timeStep );
    m_actuator_r->update( selector_input + delta_dc, m_timeStep );

    double elevator_delta_max = 60.0 * m_timeStep;
    m_delta_htl = getSurfaceMaxRate( m_delta_htl, m_actuator_l->getValue(), elevator_delta_max );
    m_delta_htr = getSurfaceMaxRate( m_delta_htr, m_actuator_r->getValue(), elevator_delta_max );

    m_delta_htl = Misc::satur( -m_elevator_max_deg, m_elevator_max_deg, m_delta_htl );
    m_delta_htr = Misc::satur( -m_elevator_max_deg, m_elevator_max_deg, m_delta_htr );

    m_delta_h = 0.5 * ( m_delta_htl + m_delta_htr );
    m_delta_d = 0.5 * ( m_delta_htr - m_delta_htl );

    m_elevator = Units::deg2rad( m_delta_h );
    m_elevator_norm = m_elevator / m_elevator_max;

    m_elevons = Units::deg2rad( m_delta_d );
}

////////////////////////////////////////////////////////////////////////////////

void F16C_FLCS::updateYaw( double ctrlYaw, double trimYaw,
                           double yawRate,
                           double q_p, double g_y )
{
    double omg_r_deg = Units::rad2deg( yawRate  );

    m_pedals->update( ctrlYaw, m_timeStep );
    m_omg_r_lag->update( omg_r_deg, m_timeStep );
    m_omg_p_yaw->update( m_omg_p_lag->getValue(), m_timeStep );

    double r_com = m_pedals->getValue() + trimYaw;
    double u_sum = m_omg_r_lag->getValue() - ( 1.0 / 57.3 ) * m_omg_p_yaw->getValue() * m_alpha_lag->getValue();

    m_u_sum_ll1->update( u_sum, m_timeStep );
    m_u_sum_ll2->update( m_u_sum_ll1->getValue(), m_timeStep );

    // Aileron Rudder Interconnect (ARI)
    double ari_gain = 0.0;

    // (AD-A055-417, p.20)
    // (NASA-TP-1538, p.216)
    double alpha_abs = fabs( m_alpha_lag->getValue() );
    if ( alpha_abs < 10.0 )
    {
        ari_gain = 1.0 - alpha_abs / 10.0;
    }
    double ari = 0.0375 * m_alpha_lag->getValue() - 0.65 * ari_gain * m_alpha_lag->getValue() * getGainF7( q_p );

    double u_gy = -g_y; // TODO
    double r_auto = getGainF8( q_p ) * ( m_u_sum_ll2->getValue() + 19.32 * u_gy ) + 0.5 * m_delta_ac * ari;

    double delta_rc = r_com + r_auto;

    m_delta_r_fil->update( delta_rc, m_timeStep );
    m_delta_r_lag->update( m_delta_r_fil->getValue(), m_timeStep );

    double rudder_delta_max = 120.0 * m_timeStep;
    m_delta_r = getSurfaceMaxRate( m_delta_r, m_delta_r_lag->getValue(), rudder_delta_max );
    m_delta_r = Misc::satur( -m_rudder_max_deg, m_rudder_max_deg, m_delta_r );

    m_rudder = Units::deg2rad( m_delta_r );
    m_rudder_norm = m_rudder / m_rudder_max;
}

////////////////////////////////////////////////////////////////////////////////

double F16C_FLCS::getGainF2( double q_p )
{
    double gain = 1.0;

    // (AD-A055-417, p.20 - F2)
    if ( q_p <= 0.53 )
    {
        gain = 0.5;
    }
    else if ( q_p <= 1.79 )
    {
        gain = 0.5 - 1.5 * ( q_p - 0.53 ) / ( 1.79 - 0.53 );
    }
    else
    {
        gain = -1.0;
    }

    return gain;
}

////////////////////////////////////////////////////////////////////////////////

double F16C_FLCS::getGainF3( double dynPress )
{
    double gain = 1.0;

    // (AD-A055-417, p.20 - F3)
    if ( dynPress > 143640.0 ) // 3,000 psf
    {
        gain = 0.083;
    }
    else if ( dynPress > 38304.0 ) // 800 psf
    {
        gain = 0.533 - ( 0.533 - 0.083 ) / ( 143640.0 - 38304.0 ) * ( dynPress - 38304.0 );
    }
    else if ( dynPress > 14364.0 ) // 300 psf
    {
        gain = 1.0 - ( 1.0 - 0.533 ) / ( 38304.0 - 14364.0 ) * ( dynPress - 14364.0 );
    }

    return gain;
}

////////////////////////////////////////////////////////////////////////////////

double F16C_FLCS::getGainF7( double q_p )
{
    double gain = 1.0;

    // (AD-A055-417, p.20 - F7)
    if ( q_p <= 0.187 )
    {
        gain = 0.0;
    }
    else if ( 0.187 <= q_p && q_p < 1.129 )
    {
        gain = ( q_p - 0.187 ) / ( 1.129 - 0.187 );
    }
    else if ( 1.129 <= q_p && q_p < 1.709 )
    {
        gain = 1.0 - ( q_p - 1.129 ) / ( 1.709 - 1.129 );
    }
    else
    {
        gain = 0.0;
    }

    return gain;
}

////////////////////////////////////////////////////////////////////////////////

double F16C_FLCS::getGainF8( double q_p )
{
    double gain = 1.0;

    // (AD-A055-417, p.20 - F8)
    if ( q_p < 2.04 )
    {
        gain = 0.5;
    }
    else if ( q_p < 3.23 )
    {
        gain = 0.5 + 0.5 * ( q_p - 2.04 ) / ( 3.23 - 2.04 );
    }

    return gain;
}

////////////////////////////////////////////////////////////////////////////////

double F16C_FLCS::getGainF9( double q_p )
{
    double gain = 0.0;

    // (AD-A055-417, p.20 - F9)
    if ( q_p > 1.008 )
    {
        gain = -2.0;
    }
    else if ( q_p > 0.787 )
    {
        gain = 0.0 - 2.0 * ( q_p - 0.787 ) / ( 1.008 - 0.787 );
    }

    return gain;
}

////////////////////////////////////////////////////////////////////////////////

double F16C_FLCS::getGainF10( double q_p )
{
    double gain = 0.5;

    // (AD-A055-417, p.20 - F10)
    if ( q_p > 1.132 )
    {
        gain = 0.5;
    }
    else if ( q_p > 0.694 )
    {
        gain = 0.25 + 0.25 * ( q_p - 0.694 ) / ( 1.132 - 0.694 );
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
