/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
 *
 * Creative Commons Legal Code
 *
 * CC0 1.0 Universal
 *
 *     CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND DOES NOT PROVIDE
 *     LEGAL SERVICES. DISTRIBUTION OF THIS DOCUMENT DOES NOT CREATE AN
 *     ATTORNEY-CLIENT RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS
 *     INFORMATION ON AN "AS-IS" BASIS. CREATIVE COMMONS MAKES NO WARRANTIES
 *     REGARDING THE USE OF THIS DOCUMENT OR THE INFORMATION OR WORKS
 *     PROVIDED HEREUNDER, AND DISCLAIMS LIABILITY FOR DAMAGES RESULTING FROM
 *     THE USE OF THIS DOCUMENT OR THE INFORMATION OR WORKS PROVIDED
 *     HEREUNDER.
 *
 * Statement of Purpose
 *
 * The laws of most jurisdictions throughout the world automatically confer
 * exclusive Copyright and Related Rights (defined below) upon the creator
 * and subsequent owner(s) (each and all, an "owner") of an original work of
 * authorship and/or a database (each, a "Work").
 *
 * Certain owners wish to permanently relinquish those rights to a Work for
 * the purpose of contributing to a commons of creative, cultural and
 * scientific works ("Commons") that the public can reliably and without fear
 * of later claims of infringement build upon, modify, incorporate in other
 * works, reuse and redistribute as freely as possible in any form whatsoever
 * and for any purposes, including without limitation commercial purposes.
 * These owners may contribute to the Commons to promote the ideal of a free
 * culture and the further production of creative, cultural and scientific
 * works, or to gain reputation or greater distribution for their Work in
 * part through the use and efforts of others.
 *
 * For these and/or other purposes and motivations, and without any
 * expectation of additional consideration or compensation, the person
 * associating CC0 with a Work (the "Affirmer"), to the extent that he or she
 * is an owner of Copyright and Related Rights in the Work, voluntarily
 * elects to apply CC0 to the Work and publicly distribute the Work under its
 * terms, with knowledge of his or her Copyright and Related Rights in the
 * Work and the meaning and intended legal effect of CC0 on those rights.
 *
 * 1. Copyright and Related Rights. A Work made available under CC0 may be
 * protected by copyright and related or neighboring rights ("Copyright and
 * Related Rights"). Copyright and Related Rights include, but are not
 * limited to, the following:
 *
 *   i. the right to reproduce, adapt, distribute, perform, display,
 *      communicate, and translate a Work;
 *  ii. moral rights retained by the original author(s) and/or performer(s);
 * iii. publicity and privacy rights pertaining to a person's image or
 *      likeness depicted in a Work;
 *  iv. rights protecting against unfair competition in regards to a Work,
 *      subject to the limitations in paragraph 4(a), below;
 *   v. rights protecting the extraction, dissemination, use and reuse of data
 *      in a Work;
 *  vi. database rights (such as those arising under Directive 96/9/EC of the
 *      European Parliament and of the Council of 11 March 1996 on the legal
 *      protection of databases, and under any national implementation
 *      thereof, including any amended or successor version of such
 *      directive); and
 * vii. other similar, equivalent or corresponding rights throughout the
 *      world based on applicable law or treaty, and any national
 *      implementations thereof.
 *
 * 2. Waiver. To the greatest extent permitted by, but not in contravention
 * of, applicable law, Affirmer hereby overtly, fully, permanently,
 * irrevocably and unconditionally waives, abandons, and surrenders all of
 * Affirmer's Copyright and Related Rights and associated claims and causes
 * of action, whether now known or unknown (including existing as well as
 * future claims and causes of action), in the Work (i) in all territories
 * worldwide, (ii) for the maximum duration provided by applicable law or
 * treaty (including future time extensions), (iii) in any current or future
 * medium and for any number of copies, and (iv) for any purpose whatsoever,
 * including without limitation commercial, advertising or promotional
 * purposes (the "Waiver"). Affirmer makes the Waiver for the benefit of each
 * member of the public at large and to the detriment of Affirmer's heirs and
 * successors, fully intending that such Waiver shall not be subject to
 * revocation, rescission, cancellation, termination, or any other legal or
 * equitable action to disrupt the quiet enjoyment of the Work by the public
 * as contemplated by Affirmer's express Statement of Purpose.
 *
 * 3. Public License Fallback. Should any part of the Waiver for any reason
 * be judged legally invalid or ineffective under applicable law, then the
 * Waiver shall be preserved to the maximum extent permitted taking into
 * account Affirmer's express Statement of Purpose. In addition, to the
 * extent the Waiver is so judged Affirmer hereby grants to each affected
 * person a royalty-free, non transferable, non sublicensable, non exclusive,
 * irrevocable and unconditional license to exercise Affirmer's Copyright and
 * Related Rights in the Work (i) in all territories worldwide, (ii) for the
 * maximum duration provided by applicable law or treaty (including future
 * time extensions), (iii) in any current or future medium and for any number
 * of copies, and (iv) for any purpose whatsoever, including without
 * limitation commercial, advertising or promotional purposes (the
 * "License"). The License shall be deemed effective as of the date CC0 was
 * applied by Affirmer to the Work. Should any part of the License for any
 * reason be judged legally invalid or ineffective under applicable law, such
 * partial invalidity or ineffectiveness shall not invalidate the remainder
 * of the License, and in such case Affirmer hereby affirms that he or she
 * will not (i) exercise any of his or her remaining Copyright and Related
 * Rights in the Work or (ii) assert any associated claims and causes of
 * action with respect to the Work, in either case contrary to Affirmer's
 * express Statement of Purpose.
 *
 * 4. Limitations and Disclaimers.
 *
 *  a. No trademark or patent rights held by Affirmer are waived, abandoned,
 *     surrendered, licensed or otherwise affected by this document.
 *  b. Affirmer offers the Work as-is and makes no representations or
 *     warranties of any kind concerning the Work, express, implied,
 *     statutory or otherwise, including without limitation warranties of
 *     title, merchantability, fitness for a particular purpose, non
 *     infringement, or the absence of latent or other defects, accuracy, or
 *     the present or absence of errors, whether or not discoverable, all to
 *     the greatest extent permissible under applicable law.
 *  c. Affirmer disclaims responsibility for clearing rights of other persons
 *     that may apply to the Work or any use thereof, including without
 *     limitation any person's Copyright and Related Rights in the Work.
 *     Further, Affirmer disclaims responsibility for obtaining any necessary
 *     consents, permissions or other rights required for any use of the
 *     Work.
 *  d. Affirmer understands and acknowledges that Creative Commons is not a
 *     party to this document and has no duty or obligation with respect to
 *     this CC0 or use of the Work.
 *
 ******************************************************************************/

#include <fdm_test/test_Blade.h>

#include <cstring>
#include <iostream>

#include <fdm/fdm_Log.h>
#include <fdm/utils/fdm_WGS84.h>
#include <fdm/xml/fdm_XmlDoc.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

test_Blade::test_Blade( bool ccw, int blades_count ) :
    _bladesCount ( blades_count ),
    _ccw ( ccw ),
    _blade ( FDM_NULLPTR )
{
    for ( int i = 0; i < _bladesCount; i++ )
    {
        RotorBlade *blade = new RotorBlade( _ccw );

        if ( i == 0 )
        {
            _blade = blade;
        }

        _blades.push_back( blade );
    }

    for ( int i = 0; i < VECT_SPAN; i++ )
    {
        _blade->span[ i ].visible = false;
        //strcpy( _blade->span[ i ].label, "" );
    }

    readData( "../data/test.xml" );

    for ( Blades::iterator it = _blades.begin(); it != _blades.end(); ++it )
    {
        (*it)->TEST_INIT();
    }

    _ned2enu(0,0) =  0.0;
    _ned2enu(0,1) =  1.0;
    _ned2enu(0,2) =  0.0;

    _ned2enu(1,0) =  1.0;
    _ned2enu(1,1) =  0.0;
    _ned2enu(1,2) =  0.0;

    _ned2enu(2,0) =  0.0;
    _ned2enu(2,1) =  0.0;
    _ned2enu(2,2) = -1.0;

    _enu2ned = _ned2enu.getTransposed();

    _bas2ned = Matrix3x3::identityMatrix();
    _ned2bas = Matrix3x3::identityMatrix();

    _bas2ras = Matrix3x3::identityMatrix();
    _ras2bas = Matrix3x3::identityMatrix();
}

////////////////////////////////////////////////////////////////////////////////

test_Blade::~test_Blade()
{
    FDM_DELPTR( _blade );

    for ( int i = 0; i < VECT_MAIN; i++ )
    {
        Data::get()->other.main[ i ].visible = false;

        Data::get()->other.main[ i ].b_x_enu = 0.0;
        Data::get()->other.main[ i ].b_y_enu = 0.0;
        Data::get()->other.main[ i ].b_z_enu = 0.0;

        Data::get()->other.main[ i ].v_x_enu = 0.0;
        Data::get()->other.main[ i ].v_y_enu = 0.0;
        Data::get()->other.main[ i ].v_z_enu = 0.0;

        strcpy( Data::get()->other.main[ i ].label, "" );
    }

    for ( int i = 0; i < VECT_SPAN; i++ )
    {
        Data::get()->other.span[ i ].visible = false;

        Data::get()->other.span[ i ].b_x_enu = 0.0;
        Data::get()->other.span[ i ].b_y_enu = 0.0;
        Data::get()->other.span[ i ].b_z_enu = 0.0;

        Data::get()->other.span[ i ].v_x_enu = 0.0;
        Data::get()->other.span[ i ].v_y_enu = 0.0;
        Data::get()->other.span[ i ].v_z_enu = 0.0;

        strcpy( Data::get()->other.span[ i ].label, "" );
    }

    for ( int i = 0; i < MAX_BLADES; i++ )
    {
        Data::get()->blade[ i ].beta  = 0.0;
        Data::get()->blade[ i ].theta = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////

void test_Blade::readData( const std::string &dataFile )
{
    XmlDoc doc( dataFile.c_str() );

    if ( doc.isOpen() )
    {
        XmlNode rootNode = doc.getRootNode();

        if ( rootNode.isValid() )
        {
            XmlNode nodeRotor = rootNode.getFirstChildElement( "main_rotor" );

            if ( nodeRotor.isValid() )
            {
                XmlNode nodeBlade = nodeRotor.getFirstChildElement( "blade" );

                for ( Blades::iterator it = _blades.begin(); it != _blades.end(); ++it )
                {
                    (*it)->readData( nodeBlade );
                }
            }
            else
            {
                Log::e() << "Reading file \"" << dataFile << "\" failed. Invalid \"main_rotor\" node." << std::endl;
            }
        }
        else
        {
            Log::e() << "Reading file \"" << dataFile << "\" failed. Invalid root node." << std::endl;
        }
    }
    else
    {
        Log::e() << "Reading file \"" << dataFile << "\" failed." << std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////

void test_Blade::update( double timeStep )
{
    _ned2bas = Matrix3x3( Angles( Data::get()->state.ned_phi,
                                  Data::get()->state.ned_tht,
                                  Data::get()->state.ned_psi ) );
    _bas2ned = _ned2bas.getTransposed();

    Vector3 vel_wind_ned = Matrix3x3( Angles( 0.0, 0.0, -Data::get()->state.wind_dir ) )
             * Vector3( -Data::get()->state.wind_vel, 0.0, 0.0 );
    Vector3 vel_wind_bas = _ned2bas * vel_wind_ned;
    Vector3 vel_wind_ras = _bas2ras * vel_wind_bas;

    Vector3 vel_bas = Vector3( Data::get()->state.bas_u,
                               Data::get()->state.bas_v,
                               Data::get()->state.bas_w );

    Vector3 omg_bas = Vector3( Data::get()->state.bas_p,
                               Data::get()->state.bas_q,
                               Data::get()->state.bas_r );

    Vector3 vel_ras = _bas2ras * vel_bas;
    Vector3 omg_ras = _bas2ras * omg_bas;

    Vector3 vel_air_ras = vel_ras - vel_wind_ras;
    Vector3 omg_air_ras = omg_ras;

    Vector3 acc_ras( 0.0, 0.0, 0.0 );
    Vector3 eps_ras( 0.0, 0.0, 0.0 );

    Vector3 grav_ned( 0.0, 0.0, WGS84::_g );
    Vector3 grav_bas = _ned2bas * grav_ned;
    Vector3 grav_ras = _bas2ras * grav_bas;

    std::cout << grav_ras.toString() << std::endl;

    double omega   = Data::get()->rotor.omega;
    double azimuth = Data::get()->rotor.azimuth;
    double airDensity = 1.225;

    double delta_psi = 2.0 * M_PI / (double)(_bladesCount);

    double theta_0  = Data::get()->rotor.collective;
    double theta_1c = ( _ccw ? -1.0 : 1.0 ) * Data::get()->rotor.cyclicLat;
    double theta_1s = Data::get()->rotor.cyclicLon;

    int index = 0;

    Vector3 for_ras;
    Vector3 mom_ras;

    for ( Blades::iterator it = _blades.begin(); it != _blades.end(); ++it )
    {
        (*it)->integrate( timeStep,
                          vel_air_ras,
                          omg_air_ras,
                          omg_ras,
                          acc_ras,
                          eps_ras,
                          grav_ras,
                          omega,
                          azimuth + index * delta_psi,
                          airDensity,
                          theta_0,
                          theta_1c,
                          theta_1s
                          );

        (*it)->computeForceAndMoment( vel_air_ras,
                                      omg_air_ras,
                                      omg_ras,
                                      acc_ras,
                                      eps_ras,
                                      grav_ras,
                                      omega,
                                      azimuth + index * delta_psi,
                                      airDensity,
                                      theta_0,
                                      theta_1c,
                                      theta_1s
                                      );

        for_ras += (*it)->getFor_RAS();
        mom_ras += (*it)->getMom_RAS();

        if ( index == 0 )
        {
            for ( int i = 0; i < VECT_MAIN; i++ )
            {
                Data::get()->other.main[ i ].visible = true;

                Data::get()->other.main[ i ].b_x_enu = 0.0;
                Data::get()->other.main[ i ].b_y_enu = 0.0;
                Data::get()->other.main[ i ].b_z_enu = 0.0;

                Data::get()->other.main[ i ].v_x_enu = 0.0;
                Data::get()->other.main[ i ].v_y_enu = 0.0;
                Data::get()->other.main[ i ].v_z_enu = 0.0;

                strcpy( Data::get()->other.main[ i ].label, "" );
            }

            double vect_coef = 1.0e-3;

            Vector3 v_bas = Data::get()->other.scale_main * _ras2bas * for_ras;
            Vector3 v_ned = _bas2ned * v_bas;
            Vector3 v_enu = _ned2enu * v_ned;

            Data::get()->other.main[ 0 ].v_x_enu = vect_coef * v_enu.x();
            Data::get()->other.main[ 0 ].v_y_enu = vect_coef * 0.0;
            Data::get()->other.main[ 0 ].v_z_enu = vect_coef * 0.0;

            Data::get()->other.main[ 1 ].v_x_enu = vect_coef * 0.0;
            Data::get()->other.main[ 1 ].v_y_enu = vect_coef * v_enu.y();
            Data::get()->other.main[ 1 ].v_z_enu = vect_coef * 0.0;

            Data::get()->other.main[ 2 ].v_x_enu = vect_coef * 0.0;
            Data::get()->other.main[ 2 ].v_y_enu = vect_coef * 0.0;
            Data::get()->other.main[ 2 ].v_z_enu = vect_coef * v_enu.z();

            for ( int i = 0; i < VECT_SPAN; i++ )
            {
                updateDataVect( Data::get()->other.span[ i ], _blade->span[ i ], azimuth );
            }
        }

        index++;
    }
}

////////////////////////////////////////////////////////////////////////////////

void test_Blade::updateData()
{
    int index = 0;

    for ( Blades::iterator it = _blades.begin(); it != _blades.end(); ++it )
    {
        Data::get()->blade[ index ].beta  = (*it)->getBeta();
        Data::get()->blade[ index ].theta = (*it)->getTheta();

        index++;
    }
}

////////////////////////////////////////////////////////////////////////////////

void test_Blade::updateDataVect( Data::Other::Vector &vect,
                                 const RotorBlade::Vect &out,
                                 double azimuth )
{
    bool ccw = false;

    if ( Data::get()->rotor.direction == Data::Rotor::CCW )
    {
        ccw = true;
    }

    Matrix3x3 ras2sra = _blade->getRAS2SRA( azimuth, ccw );
    Matrix3x3 sra2ras = ras2sra.getTransposed();

    Vector3 b_ras =  sra2ras * out.b_sra;
    Vector3 b_bas = _ras2bas * b_ras;
    Vector3 b_ned = _bas2ned * b_bas;
    Vector3 b_enu = _ned2enu * b_ned;

    Vector3 v_ras =  Data::get()->other.scale_span * sra2ras * out.v_sra;
    Vector3 v_bas = _ras2bas * v_ras;
    Vector3 v_ned = _bas2ned * v_bas;
    Vector3 v_enu = _ned2enu * v_ned;

    vect.visible = out.visible;

    vect.b_x_enu = b_enu.x();
    vect.b_y_enu = b_enu.y();
    vect.b_z_enu = b_enu.z();

    vect.v_x_enu = v_enu.x();
    vect.v_y_enu = v_enu.y();
    vect.v_z_enu = v_enu.z();

    strcpy( vect.label, "" );
}
