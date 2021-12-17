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

#include <gui/MainWindow.h>
#include <ui_MainWindow.h>

#include <iostream>

#include <QKeyEvent>
#include <QSettings>

#include <hid/hid_Manager.h>

#include <fdm/utils/fdm_Units.h>

#include <defs.h>

#include <gui/KeyMap.h>

////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow),

    _dialogCtrl ( nullptr ),

    _dockCtrl ( nullptr ),
    _dockData ( nullptr ),
    _dockMain ( nullptr ),
    _dockTest ( nullptr ),

    _timer ( nullptr ),
    _timeStep ( 0.0 ),

    _timerId ( 0 )
{
    _ui->setupUi( this );

    setCorner( Qt::TopLeftCorner     , Qt::LeftDockWidgetArea  );
    setCorner( Qt::BottomLeftCorner  , Qt::LeftDockWidgetArea  );
    setCorner( Qt::TopRightCorner    , Qt::RightDockWidgetArea );
    setCorner( Qt::BottomRightCorner , Qt::RightDockWidgetArea );

    _dialogCtrl = new DialogCtrl( this );

    _dockCtrl = new DockWidgetCtrl( this );
    _dockData = new DockWidgetData( this );
    _dockMain = new DockWidgetMain( this );
    _dockTest = new DockWidgetTest( this );

    _dockMain->setDockTest( _dockTest );
    _dockMain->setWidgetCGI( _ui->widgetCGI );

    addDockWidget( Qt::LeftDockWidgetArea  , _dockMain );
    addDockWidget( Qt::LeftDockWidgetArea  , _dockCtrl );

    addDockWidget( Qt::RightDockWidgetArea , _dockData );
    addDockWidget( Qt::RightDockWidgetArea , _dockTest );

    connect( _dockCtrl, SIGNAL(closed()), this, SLOT(dockCtrl_closed()) );
    connect( _dockData, SIGNAL(closed()), this, SLOT(dockData_closed()) );
    connect( _dockMain, SIGNAL(closed()), this, SLOT(dockMain_closed()) );
    connect( _dockTest, SIGNAL(closed()), this, SLOT(dockTest_closed()) );

    settingsRead();

    hid::Manager::instance()->init();

    _dialogCtrl->readData();
    _dialogCtrl->updateAssignments();

    _timer = new QElapsedTimer();
    _timer->start();

    _timerId = startTimer( 1000.0 * SIM_TIME_STEP );
}

////////////////////////////////////////////////////////////////////////////////

MainWindow::~MainWindow()
{
    killTimer( _timerId );

    settingsSave();

    if ( _dialogCtrl ) delete _dialogCtrl;
    _dialogCtrl = nullptr;

    if ( _dockCtrl ) delete _dockCtrl;
    _dockCtrl = nullptr;

    if ( _dockData ) delete _dockData;
    _dockData = nullptr;

    if ( _dockMain ) delete _dockMain;
    _dockMain = nullptr;

    if ( _dockTest ) delete _dockTest;
    _dockTest = nullptr;

    if ( _timer ) delete _timer;
    _timer = nullptr;

    if ( _ui ) delete _ui;
    _ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::keyPressEvent( QKeyEvent *event )
{
    ////////////////////////////////////
    QMainWindow::keyPressEvent( event );
    ////////////////////////////////////

    if ( !event->isAutoRepeat() )
    {
        _ui->widgetCGI->keyDn( KeyMap::remapHID( event->key() ) );
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::keyReleaseEvent( QKeyEvent *event )
{
    //////////////////////////////////////
    QMainWindow::keyReleaseEvent( event );
    //////////////////////////////////////

    if ( !event->isAutoRepeat() )
    {
        _ui->widgetCGI->keyUp( KeyMap::remapHID( event->key() ) );
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::timerEvent( QTimerEvent *event )
{
    /////////////////////////////////
    QMainWindow::timerEvent( event );
    /////////////////////////////////

    double timeStep = (double)_timer->restart() / 1000.0;
    double timeCoef = _dockMain->getTimeCoef();

    _timeStep = timeCoef * timeStep;

    hid::Manager::instance()->update( timeStep );

    updateDataBlades();

    _dockCtrl->update( _timeStep );
    //_dockData->update( _timeStep );
    _dockMain->update( _timeStep );
    _dockTest->update( _timeStep );

    Data::get()->time_step = timeStep;
    Data::get()->time_coef = timeCoef;

//    std::cout << Data::get()->rotor.collective << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::settingsRead()
{
    QSettings settings( TEST_ORG_NAME, TEST_APP_NAME );

    settings.beginGroup( "main_window" );

    restoreState( settings.value( "state" ).toByteArray() );
    restoreGeometry( settings.value( "geometry" ).toByteArray() );

    bool show_vectors_main = settings.value( "show_vectors_main", 0 ).toBool();
    bool show_vectors_span = settings.value( "show_vectors_span", 0 ).toBool();
    bool show_blades_datum = settings.value( "show_blades_datum", 0 ).toBool();
    bool show_blades_paths = settings.value( "show_blades_paths", 0 ).toBool();

    _ui->actionShowVectorsSpan->setChecked( show_vectors_span );
    _ui->actionShowVectorsMain->setChecked( show_vectors_main );
    _ui->actionShowBladesDatum->setChecked( show_blades_datum );
    _ui->actionShowBladesPaths->setChecked( show_blades_paths );

    Data::get()->other.show_vectors_span = show_vectors_span;
    Data::get()->other.show_vectors_main = show_vectors_main;
    Data::get()->other.show_blades_datum = show_blades_datum;
    Data::get()->other.show_blades_paths = show_blades_paths;

    bool visibleCtrl = settings.value( "dock_ctrl_visible", 1 ).toBool();
    bool visibleData = settings.value( "dock_data_visible", 1 ).toBool();
    bool visibleMain = settings.value( "dock_main_visible", 1 ).toBool();
    bool visibleTest = settings.value( "dock_test_visible", 0 ).toBool();

    _ui->actionDockCtrl->setChecked( visibleCtrl );
    _ui->actionDockData->setChecked( visibleData );
    _ui->actionDockMain->setChecked( visibleMain );
    _ui->actionDockTest->setChecked( visibleTest );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::settingsSave()
{
    QSettings settings( TEST_ORG_NAME, TEST_APP_NAME );

    settings.beginGroup( "main_window" );

    settings.setValue( "state", saveState() );
    settings.setValue( "geometry", saveGeometry() );

    settings.setValue( "show_vectors_span", _ui->actionShowVectorsSpan->isChecked() );
    settings.setValue( "show_vectors_main", _ui->actionShowVectorsMain->isChecked() );
    settings.setValue( "show_blades_datum", _ui->actionShowBladesDatum->isChecked() );
    settings.setValue( "show_blades_paths", _ui->actionShowBladesPaths->isChecked() );

    settings.setValue( "dock_ctrl_visible", _ui->actionDockCtrl->isChecked() ? 1 : 0 );
    settings.setValue( "dock_data_visible", _ui->actionDockData->isChecked() ? 1 : 0 );
    settings.setValue( "dock_main_visible", _ui->actionDockMain->isChecked() ? 1 : 0 );
    settings.setValue( "dock_test_visible", _ui->actionDockTest->isChecked() ? 1 : 0 );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::updateDataBlades()
{
    Data::get()->rotor.omega      = _dockCtrl->getOmega();
    Data::get()->rotor.azimuth    = _dockCtrl->getAzimuth();
    //Data::get()->rotor.collective = _dockCtrl->getCollective();
    //Data::get()->rotor.cyclicLon  = _dockCtrl->getCyclicLon();
    //Data::get()->rotor.cyclicLat  = _dockCtrl->getCyclicLat();

    const double min_collective = fdm::Units::deg2rad( MIN_COLLECTIVE_DEG );
    const double max_collective = fdm::Units::deg2rad( MAX_COLLECTIVE_DEG );
    const double max_cyclic     = fdm::Units::deg2rad( MAX_CYCLIC_DEG );
    Data::get()->rotor.collective = min_collective + ( max_collective - min_collective ) * hid::Manager::instance()->getCollective();
    Data::get()->rotor.cyclicLon  = max_cyclic * hid::Manager::instance()->getCtrlPitch();
    Data::get()->rotor.cyclicLat  = max_cyclic * hid::Manager::instance()->getCtrlRoll();

    bool ccw = _dockMain->getCCW();

    double beta_0  =  Data::get()->rotor.coningAngle;
    double beta_1c = -Data::get()->rotor.diskPitch;
    double beta_1s = ( ccw ? -1.0 : 1.0 ) * Data::get()->rotor.diskRoll;

    double theta_0  = Data::get()->rotor.collective;
    double theta_1c = ( ccw ? -1.0 : 1.0 ) * Data::get()->rotor.cyclicLat;
    double theta_1s = Data::get()->rotor.cyclicLon;

    unsigned int bladesCount = 4;
    double psiStep = 2.0*M_PI / (float)bladesCount;

    for ( unsigned int i = 0; i < bladesCount; i++ )
    {
        double psi = Data::get()->rotor.azimuth + (double)(i*psiStep);

        double cosPsi = cos( psi );
        double sinPsi = sin( psi );

        Data::get()->blade[ i ].beta  =  beta_0 +  beta_1c * cosPsi +  beta_1s * sinPsi;
        Data::get()->blade[ i ].theta = theta_0 + theta_1c * cosPsi + theta_1s * sinPsi;
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionControls_triggered()
{
    _dialogCtrl->readData();

    if ( _dialogCtrl->exec() == QDialog::Accepted )
    {
        _dialogCtrl->saveData();
        _dialogCtrl->updateAssignments();
    }
    else
    {
        _dialogCtrl->readData();
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionDockCtrl_toggled( bool checked )
{
    _dockCtrl->setVisible( checked );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionDockData_toggled( bool checked )
{
    _dockData->setVisible( checked );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionDockMain_toggled( bool checked )
{
    _dockMain->setVisible( checked );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionDockTest_toggled( bool checked )
{
    _dockTest->setVisible( checked );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionViewAft_triggered()
{
    _ui->widgetCGI->setViewAft();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionViewFwd_triggered()
{
    _ui->widgetCGI->setViewFwd();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionViewLft_triggered()
{
    _ui->widgetCGI->setViewLft();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionViewRgt_triggered()
{
    _ui->widgetCGI->setViewRgt();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionViewTop_triggered()
{
    _ui->widgetCGI->setViewTop();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionRotationStart_triggered()
{
    _ui->widgetCGI->rotationStart();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionRotationStop_triggered()
{
    _ui->widgetCGI->rotationStop();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionShowVectorsSpan_toggled(bool arg1)
{
    Data::get()->other.show_vectors_span = arg1;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionShowVectorsMain_toggled(bool arg1)
{
    Data::get()->other.show_vectors_main = arg1;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionShowBladesDatum_toggled(bool arg1)
{
    Data::get()->other.show_blades_datum = arg1;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionShowBladesPaths_toggled(bool arg1)
{
    Data::get()->other.show_blades_paths = arg1;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::dockCtrl_closed()
{
    _ui->actionDockCtrl->setChecked( false );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::dockData_closed()
{
    _ui->actionDockData->setChecked( false );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::dockMain_closed()
{
    _ui->actionDockMain->setChecked( false );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::dockTest_closed()
{
    _ui->actionDockTest->setChecked( false );
}
