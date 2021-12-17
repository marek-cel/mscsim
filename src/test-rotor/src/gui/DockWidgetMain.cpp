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

#include <gui/DockWidgetMain.h>
#include <ui_DockWidgetMain.h>

#include <QSettings>

#include <fdm/fdm_Log.h>
#include <fdm/utils/fdm_Units.h>

#include <Data.h>
#include <defs.h>

////////////////////////////////////////////////////////////////////////////////

DockWidgetMain::DockWidgetMain( QWidget *parent ) :
    QDockWidget ( parent ),
    _ui ( new Ui::DockWidgetMain ),
    _dockTest ( nullptr ),
    _widgetCGI ( nullptr ),
    _test ( nullptr ),
    _ccw ( false )
{
    _ui->setupUi( this );

    _test = new fdm::Test();

    settingsRead();
}

////////////////////////////////////////////////////////////////////////////////

DockWidgetMain::~DockWidgetMain()
{
    settingsSave();

    if ( _test ) delete _test;
    _test = nullptr;

    if ( _ui ) delete _ui;
    _ui = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::update( double timeStep )
{
    _ui->spinBoxTimeStep->setValue( timeStep );

    Data::get()->blades_count = _ui->spinBoxBladesCount->value();
    Data::get()->rotor.direction = _ccw ? Data::Rotor::CCW : Data::Rotor::CW;

    Data::get()->other.scale_main = 1.0;
    Data::get()->other.scale_span = _ui->spinBoxScaleSpan->value();

    if ( !Data::get()->test )
    {
        Data::get()->state.bas_u = _ui->spinBoxVelU->value();
        Data::get()->state.bas_v = _ui->spinBoxVelV->value();
        Data::get()->state.bas_w = _ui->spinBoxVelW->value();

        Data::get()->state.bas_p = fdm::Units::deg2rad( _ui->spinBoxOmgP->value() );
        Data::get()->state.bas_q = fdm::Units::deg2rad( _ui->spinBoxOmgQ->value() );
        Data::get()->state.bas_r = fdm::Units::deg2rad( _ui->spinBoxOmgR->value() );

        Data::get()->state.ned_psi = fdm::Units::deg2rad( _ui->spinBoxAttPsi->value() );
        Data::get()->state.ned_tht = fdm::Units::deg2rad( _ui->spinBoxAttTht->value() );
        Data::get()->state.ned_phi = fdm::Units::deg2rad( _ui->spinBoxAttPhi->value() );

        Data::get()->state.wind_vel = _ui->spinBoxWindVel->value();
        Data::get()->state.wind_dir = fdm::Units::deg2rad( _ui->spinBoxWindDir->value() );

        if ( _test )
        {
            if ( _ui->pushButtonWork->isChecked() )
            {
                _test->update( timeStep );
            }

            _test->updateData();
        }
        else
        {
            for ( int i = 0; i < 3; i++ )
            {

            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

double DockWidgetMain::getTimeCoef()
{
    return _ui->spinBoxTimeCoef->value();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::setDockTest( DockWidgetTest *dockTest )
{
    _dockTest = dockTest;
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::setWidgetCGI( WidgetCGI *widgetCGI )
{
    _widgetCGI = widgetCGI;
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::closeEvent( QCloseEvent *event )
{
    /////////////////////////////////
    QDockWidget::closeEvent( event );
    /////////////////////////////////

    emit closed();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::settingsRead()
{
    QSettings settings( TEST_ORG_NAME, TEST_APP_NAME );

    settings.beginGroup( "dock_main" );

    double t_coef = settings.value( "t_coef", 1.0 ).toDouble();

    _ui->spinBoxTimeCoef->setValue( t_coef );

    double scale_span = settings.value( "scale_span", 1.0 ).toDouble();

    _ui->spinBoxScaleSpan->setValue( scale_span );

    int blades_count = settings.value( "blades_count", 4 ).toInt();

    _ui->spinBoxBladesCount->setValue( blades_count );

    bool ccw = settings.value( "ccw", 0 ).toBool();

    _ui->radioButtonCW->setChecked( !ccw );
    _ui->radioButtonCCW->setChecked( ccw );

    _ccw = ccw;

    double bas_u = settings.value( "bas_u", 0.0 ).toDouble();
    double bas_v = settings.value( "bas_v", 0.0 ).toDouble();
    double bas_w = settings.value( "bas_w", 0.0 ).toDouble();

    _ui->spinBoxVelU->setValue( bas_u );
    _ui->spinBoxVelV->setValue( bas_v );
    _ui->spinBoxVelW->setValue( bas_w );

    double bas_p = settings.value( "bas_p", 0.0 ).toDouble();
    double bas_q = settings.value( "bas_q", 0.0 ).toDouble();
    double bas_r = settings.value( "bas_r", 0.0 ).toDouble();

    _ui->spinBoxOmgP->setValue( bas_p );
    _ui->spinBoxOmgQ->setValue( bas_q );
    _ui->spinBoxOmgR->setValue( bas_r );

    double ned_psi = settings.value( "ned_psi", 0.0 ).toDouble();
    double ned_tht = settings.value( "ned_tht", 0.0 ).toDouble();
    double ned_phi = settings.value( "ned_phi", 0.0 ).toDouble();

    _ui->spinBoxAttPsi->setValue( ned_psi );
    _ui->spinBoxAttTht->setValue( ned_tht );
    _ui->spinBoxAttPhi->setValue( ned_phi );

    double wind_vel = settings.value( "wind_vel", 0.0 ).toDouble();
    double wind_dir = settings.value( "wind_dir", 0.0 ).toDouble();

    _ui->spinBoxWindVel->setValue( wind_vel );
    _ui->spinBoxWindDir->setValue( wind_dir );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::settingsSave()
{
    QSettings settings( TEST_ORG_NAME, TEST_APP_NAME );

    settings.beginGroup( "dock_main" );

    settings.setValue( "t_coef", _ui->spinBoxTimeCoef->value() );

    settings.setValue( "scale_span", _ui->spinBoxScaleSpan->value() );

    settings.setValue( "blades_count", _ui->spinBoxBladesCount->value() );

    settings.setValue( "ccw", _ccw ? 1 : 0 );

    settings.setValue( "bas_u", _ui->spinBoxVelU->value() );
    settings.setValue( "bas_v", _ui->spinBoxVelV->value() );
    settings.setValue( "bas_w", _ui->spinBoxVelW->value() );

    settings.setValue( "bas_p", _ui->spinBoxOmgP->value() );
    settings.setValue( "bas_q", _ui->spinBoxOmgQ->value() );
    settings.setValue( "bas_r", _ui->spinBoxOmgR->value() );

    settings.setValue( "ned_psi", _ui->spinBoxAttPsi->value() );
    settings.setValue( "ned_tht", _ui->spinBoxAttTht->value() );
    settings.setValue( "ned_phi", _ui->spinBoxAttPhi->value() );

    settings.setValue( "wind_vel", _ui->spinBoxWindVel->value() );
    settings.setValue( "wind_dir", _ui->spinBoxWindDir->value() );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_pushButtonInit_clicked()
{
    Data::get()->phase = Data::Init;

    _ui->pushButtonWork->setEnabled( true );
    _ui->pushButtonPause->setEnabled( true );

    _ui->groupBoxDirection->setEnabled( false );

    _test->initBlade( _ccw, _ui->spinBoxBladesCount->value() );
//    _test->initRotor();

    if ( !_test->isInited() )
    {
        _ui->pushButtonStop->setChecked( true );
        _test->stop();
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_pushButtonWork_clicked()
{
    Data::get()->phase = Data::Work;

    _ui->pushButtonInit->setEnabled( false );
    _ui->pushButtonPause->setEnabled( true );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_pushButtonPause_clicked()
{
    Data::get()->phase = Data::Pause;

    _ui->pushButtonInit->setEnabled( false );
    _ui->pushButtonWork->setEnabled( true );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_pushButtonStop_clicked()
{
    Data::get()->phase = Data::Stop;

    _ui->pushButtonInit->setEnabled( true );
    _ui->pushButtonWork->setEnabled( false );
    _ui->pushButtonPause->setEnabled( false );

    _ui->groupBoxDirection->setEnabled( true );

    _test->stop();

    if ( _widgetCGI )
    {
        _widgetCGI->rotationStop();
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_radioButtonCCW_toggled(bool checked)
{
    _ccw = checked;

    if ( _dockTest )
    {
        _dockTest->setCCW( _ccw );
    }
}

