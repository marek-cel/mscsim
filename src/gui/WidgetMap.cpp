/****************************************************************************//*
 * Copyright (C) 2021 Marek M. Cel
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

#include <gui/WidgetMap.h>

#include <QAction>
#include <QContextMenuEvent>
#include <QLocale>
#include <QMenu>
#include <QSettings>

#include <osgGA/StateSetManipulator>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <cgi/cgi_Defines.h>
#include <cgi/cgi_Manager.h>
#include <cgi/cgi_Mercator.h>

#include <gui/gui_Defines.h>

////////////////////////////////////////////////////////////////////////////////

void scaleChangeCallback( double scale )
{
    cgi::Manager::instance()->setMapScale( scale );
}

////////////////////////////////////////////////////////////////////////////////

WidgetMap::WidgetMap( QWidget *parent ) :
    WidgetOSG ( parent ),
    _timerId ( 0 ),
    _camManipulatorInited ( false ),
    _viewSatellite ( false ),
    _viewCrops     ( false ),
    _viewGrassland ( false ),
    _viewWoodland  ( false ),
    _viewBuiltup   ( false ),
    _viewInWaters  ( false ),
    _viewAirports  ( true  ),
    _viewRailroads ( false ),
    _viewRoads     ( false ),
    _viewBorders   ( false ),
    _viewTraces    ( true  )
{
    setMouseTracking( true );

    _manipulator = new cgi::ManipulatorMap();
    _manipulator->registerScaleChangeCallback( &scaleChangeCallback );
    _manipulator->setScaleMin( 1.0e-5 );//m_manipulator->setScaleMin( 5.0e-5 );
    _manipulator->setScaleMax( 1.0 );
    _manipulator->setMapHeight( 2.0 * CGI_MAP_Y_2 );
    _manipulator->setMapMinX( -cgi::Mercator::_max_x );
    _manipulator->setMapMaxX(  cgi::Mercator::_max_x );
    _manipulator->setMapMinY( -cgi::Mercator::_max_y );
    _manipulator->setMapMaxY(  cgi::Mercator::_max_y );
    _manipulator->setAllowThrow( true );
    _manipulator->setScale( 1.0e-3 );

#   ifdef USE_OSGQOPENGL
    QObject::connect( this, &osgQOpenGLWidget::initialized, [ this ]
    {
        getOsgViewer()->setThreadingModel( osgViewer::ViewerBase::SingleThreaded );
        //getOsgViewer()->setThreadingModel( osgViewer::ViewerBase::ThreadPerContext );

        createCameraMap();

        // add the state manipulator
        getOsgViewer()->addEventHandler( new osgGA::StateSetManipulator( getOsgViewer()->getCamera()->getOrCreateStateSet() ) );

        // add the thread model handler
        getOsgViewer()->addEventHandler( new osgViewer::ThreadingHandler );

        // add the window size toggle handler
        getOsgViewer()->addEventHandler( new osgViewer::WindowSizeHandler );

        // add the stats handler
        getOsgViewer()->addEventHandler( new osgViewer::StatsHandler );

        // add the record camera path handler
        getOsgViewer()->addEventHandler( new osgViewer::RecordCameraPathHandler );

        // add the LOD Scale handler
        getOsgViewer()->addEventHandler( new osgViewer::LODScaleHandler );

        // add the screen capture handler
        getOsgViewer()->addEventHandler( new osgViewer::ScreenCaptureHandler );

        getOsgViewer()->setKeyEventSetsDone( 0 );

        /////////////////////////////////////////////////
        getOsgViewer()->setSceneData( new osg::Group() );
        /////////////////////////////////////////////////

        getOsgViewer()->assignSceneDataToCameras();

        _initialized = true;
    });
#   else
    QWidget *widget = addViewWidget();

    _layout = new QGridLayout( this );
    _layout->setContentsMargins( 1, 1, 1, 1 );
    _layout->addWidget( widget, 0, 0 );

    setLayout( _layout );
#   endif

    settingsRead();

    cgi::Manager::instance()->setVisibilitySatellite ( _viewSatellite );
    cgi::Manager::instance()->setVisibilityCrops     ( _viewCrops     );
    cgi::Manager::instance()->setVisibilityGrassland ( _viewGrassland );
    cgi::Manager::instance()->setVisibilityWoodland  ( _viewWoodland  );
    cgi::Manager::instance()->setVisibilityBuiltup   ( _viewBuiltup   );
    cgi::Manager::instance()->setVisibilityInWaters  ( _viewInWaters  );
    cgi::Manager::instance()->setVisibilityAirports  ( _viewAirports  );
    cgi::Manager::instance()->setVisibilityRailroads ( _viewRailroads );
    cgi::Manager::instance()->setVisibilityRoads     ( _viewRoads     );
    cgi::Manager::instance()->setVisibilityBorders   ( _viewBorders   );
    cgi::Manager::instance()->setVisibilityTraces    ( _viewTraces    );

    _timerId = startTimer( 1000.0 * GUI_TIME_STEP );
}

////////////////////////////////////////////////////////////////////////////////

WidgetMap::~WidgetMap()
{
    if ( _timerId ) killTimer( _timerId );

    settingsSave();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::contextMenuEvent( QContextMenuEvent *event )
{
    QMenu menuContext( this );

    QMenu menuLayers( this );
    QMenu menuTraces( this );

    menuContext.addMenu( &menuLayers );
    menuContext.addMenu( &menuTraces );

    menuLayers.setTitle( "Layers" );
    menuTraces.setTitle( "Traces" );

    QAction actionViewSatellite ( this );
    QAction actionViewCrops     ( this );
    QAction actionViewGrassland ( this );
    QAction actionViewWoodland  ( this );
    QAction actionViewBuiltup   ( this );
    QAction actionViewInWaters  ( this );
    QAction actionViewAirports  ( this );
    QAction actionViewRailroads ( this );
    QAction actionViewRoads     ( this );
    QAction actionViewBorders   ( this );
    QAction actionViewTraces    ( this );
    QAction actionTracesReset   ( this );
    QAction actionCenterView    ( this );

    actionViewSatellite .setText( "Show/Hide Satellite" );
    actionViewCrops     .setText( "Show/Hide Crops" );
    actionViewGrassland .setText( "Show/Hide Grass" );
    actionViewWoodland  .setText( "Show/Hide Forests" );
    actionViewBuiltup   .setText( "Show/Hide Cities" );
    actionViewInWaters  .setText( "Show/Hide Inland Waters" );
    actionViewAirports  .setText( "Show/Hide Airports" );
    actionViewRailroads .setText( "Show/Hide Railroads" );
    actionViewRoads     .setText( "Show/Hide Roads" );
    actionViewBorders   .setText( "Show/Hide Borders" );
    actionViewTraces    .setText( "Show/Hide Traces" );
    actionTracesReset   .setText( "Reset Traces" );
    actionCenterView    .setText( "Center View" );

    actionViewSatellite .setCheckable( true );
    actionViewCrops     .setCheckable( true );
    actionViewGrassland .setCheckable( true );
    actionViewWoodland  .setCheckable( true );
    actionViewBuiltup   .setCheckable( true );
    actionViewInWaters  .setCheckable( true );
    actionViewAirports  .setCheckable( true );
    actionViewRailroads .setCheckable( true );
    actionViewRoads     .setCheckable( true );
    actionViewBorders   .setCheckable( true );
    actionViewTraces    .setCheckable( true );

    actionViewSatellite .setChecked( _viewSatellite );
    actionViewCrops     .setChecked( _viewCrops     );
    actionViewGrassland .setChecked( _viewGrassland );
    actionViewWoodland  .setChecked( _viewWoodland  );
    actionViewBuiltup   .setChecked( _viewBuiltup   );
    actionViewInWaters  .setChecked( _viewInWaters  );
    actionViewAirports  .setChecked( _viewAirports  );
    actionViewRailroads .setChecked( _viewRailroads );
    actionViewRoads     .setChecked( _viewRoads     );
    actionViewBorders   .setChecked( _viewBorders   );
    actionViewTraces    .setChecked( _viewTraces    );

    connect( &actionViewSatellite , SIGNAL( toggled(bool) ), this, SLOT( actionViewSatellite_toggled (bool) ) );
    connect( &actionViewCrops     , SIGNAL( toggled(bool) ), this, SLOT( actionViewCrops_toggled     (bool) ) );
    connect( &actionViewGrassland , SIGNAL( toggled(bool) ), this, SLOT( actionViewGrassland_toggled (bool) ) );
    connect( &actionViewWoodland  , SIGNAL( toggled(bool) ), this, SLOT( actionViewWoodland_toggled  (bool) ) );
    connect( &actionViewBuiltup   , SIGNAL( toggled(bool) ), this, SLOT( actionViewBuiltup_toggled   (bool) ) );
    connect( &actionViewInWaters  , SIGNAL( toggled(bool) ), this, SLOT( actionViewInWaters_toggled  (bool) ) );
    connect( &actionViewAirports  , SIGNAL( toggled(bool) ), this, SLOT( actionViewAirports_toggled  (bool) ) );
    connect( &actionViewRailroads , SIGNAL( toggled(bool) ), this, SLOT( actionViewRailroads_toggled (bool) ) );
    connect( &actionViewRoads     , SIGNAL( toggled(bool) ), this, SLOT( actionViewRoads_toggled     (bool) ) );
    connect( &actionViewBorders   , SIGNAL( toggled(bool) ), this, SLOT( actionViewBorders_toggled   (bool) ) );
    connect( &actionViewTraces    , SIGNAL( toggled(bool) ), this, SLOT( actionViewTraces_toggled    (bool) ) );

    connect( &actionTracesReset, SIGNAL( triggered() ), this, SLOT( actionTracesReset_triggered() ) );

    connect( &actionCenterView, SIGNAL( triggered() ), this, SLOT( actionCenterView_triggered() ) );

    menuLayers.addAction( &actionViewSatellite );
    //menuLayers.addAction( &actionViewCrops     );
    //menuLayers.addAction( &actionViewGrassland );
    menuLayers.addAction( &actionViewWoodland  );
    menuLayers.addAction( &actionViewBuiltup   );
    menuLayers.addAction( &actionViewInWaters  );
    menuLayers.addAction( &actionViewAirports  );
    menuLayers.addAction( &actionViewRailroads );
    menuLayers.addAction( &actionViewRoads     );
    menuLayers.addAction( &actionViewBorders   );

    menuTraces.addAction( &actionViewTraces  );
    menuTraces.addAction( &actionTracesReset );

    menuContext.addAction( &actionCenterView );

    menuContext.exec( event->globalPos() );

    actionViewSatellite .disconnect();
    actionViewCrops     .disconnect();
    actionViewGrassland .disconnect();
    actionViewWoodland  .disconnect();
    actionViewBuiltup   .disconnect();
    actionViewInWaters  .disconnect();
    actionViewAirports  .disconnect();
    actionViewRailroads .disconnect();
    actionViewRoads     .disconnect();
    actionViewBorders   .disconnect();
    actionViewTraces    .disconnect();
    actionTracesReset   .disconnect();
    actionCenterView    .disconnect();
}

////////////////////////////////////////////////////////////////////////////////

bool WidgetMap::event( QEvent *event )
{
    ///////////////////////////////
    return QWidget::event( event );
    ///////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::timerEvent( QTimerEvent *event )
{
    /////////////////////////////
    QWidget::timerEvent( event );
    /////////////////////////////

    update();

    cgi::Manager::instance()->updateMap();

    updateMouseGeoPositionStr( _manipulator->getMouseLat(),
                               _manipulator->getMouseLon() );

    emit mouseMoveGeoPosition( _mouseGeoPositionStr );
}

////////////////////////////////////////////////////////////////////////////////

#ifndef USE_OSGQOPENGL
QWidget* WidgetMap::addViewWidget()
{
    createCameraMap();

    setSceneData( cgi::Manager::instance()->getNodeMap() );
    addEventHandler( new osgViewer::StatsHandler );
    setCameraManipulator( _manipulator.get() );

    setKeyEventSetsDone( 0 );

    assignSceneDataToCameras();

    return _gwin->getGLWidget();
}
#endif

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::createCameraMap()
{
    osg::ref_ptr<osg::Camera> cameraMap = getOsgViewer()->getCamera();

#   ifndef USE_OSGQOPENGL
    cameraMap->setGraphicsContext( _gwin );
#   endif

    const osg::GraphicsContext::Traits *traits = cameraMap->getGraphicsContext()->getTraits();

    double w2h = (double)(traits->width) / (double)(traits->height);

    cameraMap->setClearColor( osg::Vec4( cgi::Map::_colorOceans, 1.0 ) );
    //cameraMap->setClearColor( osg::Vec4( 0.0, 0.0, 0.0, 1.0 ) );
    cameraMap->setViewport( new osg::Viewport( 0, 0, traits->width, traits->height ) );

    if ( 0 )
    {
        cameraMap->setProjectionMatrixAsPerspective( CGI_MAP_FOV_Y, w2h, 1.0, 10000.0 );
    }
    else
    {
        cameraMap->setViewport( new osg::Viewport( 0, 0, traits->width, traits->height ) );
        cameraMap->setProjectionMatrixAsOrtho2D( -CGI_MAP_Y_2 * w2h, CGI_MAP_Y_2 * w2h, -CGI_MAP_Y_2, CGI_MAP_Y_2 );
        cameraMap->setViewMatrix( osg::Matrix::identity() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::updateMouseGeoPositionStr( double lat, double lon )
{
    double lat_deg = osg::RadiansToDegrees( lat );
    double lon_deg = osg::RadiansToDegrees( lon );

    double lat_deg_abs = fabs( lat_deg );
    double lon_deg_abs = fabs( lon_deg );

    int lat_d = floor( lat_deg_abs );
    int lat_m = floor( 60.0 * ( lat_deg_abs - lat_d ) );
    double lat_s = 3600.0 * ( lat_deg_abs - lat_d - lat_m / 60.0 );

    int lon_d = floor( lon_deg_abs );
    int lon_m = floor( 60.0 * ( lon_deg_abs - lon_d ) );
    double lon_s = 3600.0 * ( lon_deg_abs - lon_d - lon_m / 60.0 );

    _mouseGeoPositionStr.clear();

    _mouseGeoPositionStr += QString("%1").arg( lon_d, 3, 'f', 0, QChar(' ') );
    _mouseGeoPositionStr += QString::fromUtf8( "° " );
    _mouseGeoPositionStr += QString("%1").arg( lon_m, 2, 'f', 0, QChar('0') );
    _mouseGeoPositionStr += "' ";
    _mouseGeoPositionStr += QString("%1").arg( lon_s, 5, 'f', 2, QChar('0') );
    _mouseGeoPositionStr += "\"";
    _mouseGeoPositionStr += ( lon > 0.0 ) ? "E" : "W";

    _mouseGeoPositionStr += "  ;  ";

    _mouseGeoPositionStr += QString("%1").arg( lat_d, 2, 'f', 0, QChar(' ') );
    _mouseGeoPositionStr += QString::fromUtf8( "° " );
    _mouseGeoPositionStr += QString("%1").arg( lat_m, 2, 'f', 0, QChar('0') );
    _mouseGeoPositionStr += "' ";
    _mouseGeoPositionStr += QString("%1").arg( lat_s, 5, 'f', 2, QChar('0') );
    _mouseGeoPositionStr += "\"";
    _mouseGeoPositionStr += ( lat > 0.0 ) ? "N" : "S";

    _mouseGeoPositionStr += " (";
    _mouseGeoPositionStr += QString::number( lon_deg, 'f', 6 );
    _mouseGeoPositionStr += "  ;  ";
    _mouseGeoPositionStr += QString::number( lat_deg, 'f', 6 );
    _mouseGeoPositionStr += ")";
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::settingsRead()
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );

    settings.beginGroup( "widget_map" );

    _viewSatellite = settings.value( "view_satellite" , _viewSatellite ).toBool();
    _viewCrops     = settings.value( "view_crops"     , _viewCrops     ).toBool();
    _viewGrassland = settings.value( "view_grassland" , _viewGrassland ).toBool();
    _viewWoodland  = settings.value( "view_woodland"  , _viewWoodland  ).toBool();
    _viewBuiltup   = settings.value( "view_builtup"   , _viewBuiltup   ).toBool();
    _viewInWaters  = settings.value( "inland_waters"  , _viewInWaters  ).toBool();
    _viewAirports  = settings.value( "view_airports"  , _viewAirports  ).toBool();
    _viewRailroads = settings.value( "view_railroads" , _viewRailroads ).toBool();
    _viewRoads     = settings.value( "view_roads"     , _viewRoads     ).toBool();
    _viewBorders   = settings.value( "view_borders"   , _viewBorders   ).toBool();
    _viewTraces    = settings.value( "view_traces"    , _viewTraces    ).toBool();

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::settingsSave()
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );

    settings.beginGroup( "widget_map" );

    settings.setValue( "view_satellite" , _viewSatellite ? 1 : 0 );
    settings.setValue( "view_crops"     , _viewCrops     ? 1 : 0 );
    settings.setValue( "view_grassland" , _viewGrassland ? 1 : 0 );
    settings.setValue( "view_woodland"  , _viewWoodland  ? 1 : 0 );
    settings.setValue( "view_builtup"   , _viewBuiltup   ? 1 : 0 );
    settings.setValue( "inland_waters"  , _viewInWaters  ? 1 : 0 );
    settings.setValue( "view_airports"  , _viewAirports  ? 1 : 0 );
    settings.setValue( "view_railroads" , _viewRailroads ? 1 : 0 );
    settings.setValue( "view_roads"     , _viewRoads     ? 1 : 0 );
    settings.setValue( "view_borders"   , _viewBorders   ? 1 : 0 );
    settings.setValue( "view_traces"    , _viewTraces    ? 1 : 0 );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::actionViewSatellite_toggled( bool checked )
{
    _viewSatellite = checked;
    cgi::Manager::instance()->setVisibilitySatellite( checked );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::actionViewCrops_toggled( bool checked )
{
    _viewCrops = checked;
    cgi::Manager::instance()->setVisibilityCrops( checked );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::actionViewGrassland_toggled( bool checked )
{
    _viewGrassland = checked;
    cgi::Manager::instance()->setVisibilityGrassland( checked );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::actionViewWoodland_toggled( bool checked )
{
    _viewWoodland = checked;
    cgi::Manager::instance()->setVisibilityWoodland( checked );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::actionViewBuiltup_toggled( bool checked )
{
    _viewBuiltup = checked;
    cgi::Manager::instance()->setVisibilityBuiltup( checked );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::actionViewInWaters_toggled( bool checked )
{
    _viewInWaters = checked;
    cgi::Manager::instance()->setVisibilityInWaters( _viewInWaters );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::actionViewAirports_toggled( bool checked )
{
    _viewAirports = checked;
    cgi::Manager::instance()->setVisibilityAirports( checked );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::actionViewRailroads_toggled( bool checked )
{
    _viewRailroads = checked;
    cgi::Manager::instance()->setVisibilityRailroads( checked );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::actionViewRoads_toggled( bool checked )
{
    _viewRoads = checked;
    cgi::Manager::instance()->setVisibilityRoads( checked );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::actionViewBorders_toggled( bool checked )
{
    _viewBorders = checked;
    cgi::Manager::instance()->setVisibilityBorders( checked );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::actionViewTraces_toggled( bool checked )
{
    _viewTraces = checked;
    cgi::Manager::instance()->setVisibilityTraces( checked );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::actionTracesReset_triggered()
{
    cgi::Manager::instance()->resetTraces();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::actionCenterView_triggered()
{
    _manipulator->setCenterX( cgi::Mercator::x( Data::get()->ownship.longitude ) );
    _manipulator->setCenterY( cgi::Mercator::y( Data::get()->ownship.latitude  ) );
}
