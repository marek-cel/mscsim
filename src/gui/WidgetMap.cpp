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

#include <gui/WidgetMap.h>

#include <QAction>
#include <QContextMenuEvent>
#include <QMenu>
#include <QSettings>

#include <osgDB/ReadFile>
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
    QWidget ( parent ),
    m_gridLayout ( 0 ),
    m_timerId ( 0 ),
    m_camManipulatorInited ( false ),
    m_viewCrops     ( false ),
    m_viewGrassland ( false ),
    m_viewWoodland  ( false ),
    m_viewBuiltup   ( false ),
    m_viewRailroads ( false ),
    m_viewRoads     ( false ),
    m_viewAirports  ( true  ),
    m_viewSatellite ( false ),
    m_viewBorders   ( false ),
    m_viewTraces    ( true  )
{
    setMouseTracking( true );

    setThreadingModel( osgViewer::ViewerBase::SingleThreaded );
    //setThreadingModel( osgViewer::ViewerBase::ThreadPerContext );

    m_graphicsWindow = createGraphicsWindow( x(), y(), width(), height() );

    m_manipulator = new cgi::ManipulatorMap();
    m_manipulator->registerScaleChangeCallback( &scaleChangeCallback );
    m_manipulator->setScaleMin( 1.0e-5 );//m_manipulator->setScaleMin( 5.0e-5 );
    m_manipulator->setScaleMax( 1.0 );
    m_manipulator->setMapHeight( 2.0 * CGI_MAP_Y_2 );
    m_manipulator->setMapMinX( -cgi::Mercator::max_x );
    m_manipulator->setMapMaxX(  cgi::Mercator::max_x );
    m_manipulator->setMapMinY( -cgi::Mercator::max_y );
    m_manipulator->setMapMaxY(  cgi::Mercator::max_y );
    m_manipulator->setAllowThrow( true );

    QWidget *widget = addViewWidget();

    m_gridLayout = new QGridLayout( this );
    m_gridLayout->setContentsMargins( 1, 1, 1, 1 );
    m_gridLayout->addWidget( widget, 0, 0 );

    setLayout( m_gridLayout );

    settingsRead();

    cgi::Manager::instance()->setVisibilityCrops     ( m_viewCrops     );
    cgi::Manager::instance()->setVisibilityGrassland ( m_viewGrassland );
    cgi::Manager::instance()->setVisibilityWoodland  ( m_viewWoodland  );
    cgi::Manager::instance()->setVisibilityBuiltup   ( m_viewBuiltup   );
    cgi::Manager::instance()->setVisibilityRailroads ( m_viewRailroads );
    cgi::Manager::instance()->setVisibilityRoads     ( m_viewRoads     );
    cgi::Manager::instance()->setVisibilityAirports  ( m_viewAirports  );
    cgi::Manager::instance()->setVisibilitySatellite ( m_viewSatellite );
    cgi::Manager::instance()->setVisibilityBorders   ( m_viewBorders   );
    cgi::Manager::instance()->setVisibilityTraces    ( m_viewTraces    );

    m_timerId = startTimer( GUI_TIME_STEP );
}

////////////////////////////////////////////////////////////////////////////////

WidgetMap::~WidgetMap()
{
    if ( m_timerId ) killTimer( m_timerId );

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

    QAction actionViewCrops     ( this );
    QAction actionViewGrassland ( this );
    QAction actionViewWoodland  ( this );
    QAction actionViewBuiltup   ( this );
    QAction actionViewRailroads ( this );
    QAction actionViewRoads     ( this );
    QAction actionViewAirports  ( this );
    QAction actionViewSatellite ( this );
    QAction actionViewBorders   ( this );
    QAction actionViewTraces    ( this );
    QAction actionTracesReset   ( this );

    actionViewCrops     .setText( "Show/Hide Crops" );
    actionViewGrassland .setText( "Show/Hide Grass" );
    actionViewWoodland  .setText( "Show/Hide Forests" );
    actionViewBuiltup   .setText( "Show/Hide Cities" );
    actionViewRailroads .setText( "Show/Hide Railroads" );
    actionViewRoads     .setText( "Show/Hide Roads" );
    actionViewAirports  .setText( "Show/Hide Airports" );
    actionViewSatellite .setText( "Show/Hide Satellite" );
    actionViewBorders   .setText( "Show/Hide Borders" );
    actionViewTraces    .setText( "Show/Hide Traces" );
    actionTracesReset   .setText( "Reset Traces" );

    actionViewCrops     .setCheckable( true );
    actionViewGrassland .setCheckable( true );
    actionViewWoodland  .setCheckable( true );
    actionViewBuiltup   .setCheckable( true );
    actionViewRailroads .setCheckable( true );
    actionViewRoads     .setCheckable( true );
    actionViewAirports  .setCheckable( true );
    actionViewSatellite .setCheckable( true );
    actionViewBorders   .setCheckable( true );
    actionViewTraces    .setCheckable( true );

    actionViewCrops     .setChecked( m_viewCrops     );
    actionViewGrassland .setChecked( m_viewGrassland );
    actionViewWoodland  .setChecked( m_viewWoodland  );
    actionViewBuiltup   .setChecked( m_viewBuiltup   );
    actionViewRailroads .setChecked( m_viewRailroads );
    actionViewRoads     .setChecked( m_viewRoads     );
    actionViewAirports  .setChecked( m_viewAirports  );
    actionViewSatellite .setChecked( m_viewSatellite );
    actionViewBorders   .setChecked( m_viewBorders   );
    actionViewTraces    .setChecked( m_viewTraces    );

    connect( &actionViewCrops     , SIGNAL( toggled(bool) ), this, SLOT( actionViewCrops_toggled     (bool) ) );
    connect( &actionViewGrassland , SIGNAL( toggled(bool) ), this, SLOT( actionViewGrassland_toggled (bool) ) );
    connect( &actionViewWoodland  , SIGNAL( toggled(bool) ), this, SLOT( actionViewWoodland_toggled  (bool) ) );
    connect( &actionViewBuiltup   , SIGNAL( toggled(bool) ), this, SLOT( actionViewBuiltup_toggled   (bool) ) );
    connect( &actionViewRailroads , SIGNAL( toggled(bool) ), this, SLOT( actionViewRailroads_toggled (bool) ) );
    connect( &actionViewRoads     , SIGNAL( toggled(bool) ), this, SLOT( actionViewRoads_toggled     (bool) ) );
    connect( &actionViewAirports  , SIGNAL( toggled(bool) ), this, SLOT( actionViewAirports_toggled  (bool) ) );
    connect( &actionViewSatellite , SIGNAL( toggled(bool) ), this, SLOT( actionViewSatellite_toggled (bool) ) );
    connect( &actionViewBorders   , SIGNAL( toggled(bool) ), this, SLOT( actionViewBorders_toggled   (bool) ) );
    connect( &actionViewTraces    , SIGNAL( toggled(bool) ), this, SLOT( actionViewTraces_toggled    (bool) ) );

    connect( &actionTracesReset, SIGNAL( triggered() ), this, SLOT( actionTracesReset_triggered() ) );

//    menuLayers.addAction( &actionViewCrops     );
//    menuLayers.addAction( &actionViewGrassland );
//    menuLayers.addAction( &actionViewWoodland  );
//    menuLayers.addAction( &actionViewBuiltup   );
//    menuLayers.addAction( &actionViewRailroads );
//    menuLayers.addAction( &actionViewRoads     );
    menuLayers.addAction( &actionViewAirports  );
    menuLayers.addAction( &actionViewSatellite );
//    menuLayers.addAction( &actionViewBorders   );

    menuTraces.addAction( &actionViewTraces  );
    menuTraces.addAction( &actionTracesReset );

    menuContext.exec( event->globalPos() );

    actionViewCrops     .disconnect();
    actionViewGrassland .disconnect();
    actionViewWoodland  .disconnect();
    actionViewBuiltup   .disconnect();
    actionViewRailroads .disconnect();
    actionViewRoads     .disconnect();
    actionViewAirports  .disconnect();
    actionViewSatellite .disconnect();
    actionViewBorders   .disconnect();
    actionViewTraces    .disconnect();
    actionTracesReset   .disconnect();
}

////////////////////////////////////////////////////////////////////////////////

bool WidgetMap::event( QEvent *event )
{
    ///////////////////////////////
    return QWidget::event( event );
    ///////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::paintEvent( QPaintEvent *event )
{
    /////////////////////////////
    QWidget::paintEvent( event );
    /////////////////////////////

    frame();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::timerEvent( QTimerEvent *event )
{
    /////////////////////////////
    QWidget::timerEvent( event );
    /////////////////////////////

    update();

    cgi::Manager::instance()->updateMap();

    updateMouseGeoPositionStr( m_manipulator->getMouseLat(),
                               m_manipulator->getMouseLon() );

    emit mouseMoveGeoPosition( m_mouseGeoPositionStr );
}

////////////////////////////////////////////////////////////////////////////////

QWidget* WidgetMap::addViewWidget()
{
    createCameraMap();

    setSceneData( cgi::Manager::instance()->getNodeMap() );
    addEventHandler( new osgViewer::StatsHandler );
    setCameraManipulator( m_manipulator.get() );

    setKeyEventSetsDone( 0 );

    assignSceneDataToCameras();

    return m_graphicsWindow->getGLWidget();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::createCameraMap()
{
    osg::ref_ptr<osg::Camera> cameraMap = getCamera();

    cameraMap->setGraphicsContext( m_graphicsWindow );

    const osg::GraphicsContext::Traits *traits = m_graphicsWindow->getTraits();

    double w2h = (double)(traits->width) / (double)(traits->height);

    cameraMap->setClearColor( osg::Vec4( cgi::Map::colorOceans, 1.0 ) );
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

osg::ref_ptr<osgQt::GraphicsWindowQt> WidgetMap::createGraphicsWindow( int x, int y, int w, int h,
                                                                       const std::string &name,
                                                                       bool windowDecoration )
{
    osg::DisplaySettings *displaySettings = osg::DisplaySettings::instance().get();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();

    traits->windowName       = name;
    traits->windowDecoration = windowDecoration;
    traits->x                = x;
    traits->y                = y;
    traits->width            = w;
    traits->height           = h;
    traits->doubleBuffer     = true;
    traits->alpha            = displaySettings->getMinimumNumAlphaBits();
    traits->stencil          = displaySettings->getMinimumNumStencilBits();
    traits->sampleBuffers    = displaySettings->getMultiSamples();
    traits->samples          = 4;
#   ifdef SIM_VERTICALSYNC
    traits->vsync            = true;
#   else
    traits->vsync            = false;
#   endif

    osg::ref_ptr<osgQt::GraphicsWindowQt> graphicsWindow = new osgQt::GraphicsWindowQt( traits.get() );

    return graphicsWindow;
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::updateMouseGeoPositionStr( double lat, double lon )
{
    double lat_deg = osg::RadiansToDegrees( fabs( lat ) );
    double lon_deg = osg::RadiansToDegrees( fabs( lon ) );

    int lat_d = floor( lat_deg );
    int lat_m = floor( 60.0 * ( lat_deg - lat_d ) );
    double lat_s = 3600.0 * ( lat_deg - lat_d - lat_m / 60.0 );

    int lon_d = floor( lon_deg );
    int lon_m = floor( 60.0 * ( lon_deg - lon_d ) );
    double lon_s = 3600.0 * ( lon_deg - lon_d - lon_m / 60.0 );

    m_mouseGeoPositionStr.clear();

    m_mouseGeoPositionStr += QString("%1").arg( lon_d, 3, 'f', 0, QChar(' ') );
    m_mouseGeoPositionStr += QString::fromUtf8( "° " );
    m_mouseGeoPositionStr += QString("%1").arg( lon_m, 2, 'f', 0, QChar('0') );
    m_mouseGeoPositionStr += "' ";
    m_mouseGeoPositionStr += QString("%1").arg( lon_s, 5, 'f', 2, QChar('0') );
    m_mouseGeoPositionStr += "\"";
    m_mouseGeoPositionStr += ( lon > 0.0 ) ? "E" : "W";

    m_mouseGeoPositionStr += ",  ";

    m_mouseGeoPositionStr += QString("%1").arg( lat_d, 2, 'f', 0, QChar(' ') );
    m_mouseGeoPositionStr += QString::fromUtf8( "° " );
    m_mouseGeoPositionStr += QString("%1").arg( lat_m, 2, 'f', 0, QChar('0') );
    m_mouseGeoPositionStr += "' ";
    m_mouseGeoPositionStr += QString("%1").arg( lat_s, 5, 'f', 2, QChar('0') );
    m_mouseGeoPositionStr += "\"";
    m_mouseGeoPositionStr += ( lat > 0.0 ) ? "N" : "S";
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::settingsRead()
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );

    settings.beginGroup( "widget_map" );

    m_viewCrops     = settings.value( "view_crops"     , m_viewCrops     ).toBool();
    m_viewGrassland = settings.value( "view_grassland" , m_viewGrassland ).toBool();
    m_viewWoodland  = settings.value( "view_woodland"  , m_viewWoodland  ).toBool();
    m_viewBuiltup   = settings.value( "view_builtup"   , m_viewBuiltup   ).toBool();
    m_viewRailroads = settings.value( "view_railroads" , m_viewRailroads ).toBool();
    m_viewRoads     = settings.value( "view_roads"     , m_viewRoads     ).toBool();
    m_viewAirports  = settings.value( "view_airports"  , m_viewAirports  ).toBool();
    m_viewSatellite = settings.value( "view_satellite" , m_viewSatellite ).toBool();
    m_viewBorders   = settings.value( "view_borders"   , m_viewBorders   ).toBool();
    m_viewTraces    = settings.value( "view_traces"    , m_viewTraces    ).toBool();

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::settingsSave()
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );

    settings.beginGroup( "widget_map" );

    settings.setValue( "view_crops"     , m_viewCrops     ? 1 : 0 );
    settings.setValue( "view_grassland" , m_viewGrassland ? 1 : 0 );
    settings.setValue( "view_woodland"  , m_viewWoodland  ? 1 : 0 );
    settings.setValue( "view_builtup"   , m_viewBuiltup   ? 1 : 0 );
    settings.setValue( "view_railroads" , m_viewRailroads ? 1 : 0 );
    settings.setValue( "view_roads"     , m_viewRoads     ? 1 : 0 );
    settings.setValue( "view_airports"  , m_viewAirports  ? 1 : 0 );
    settings.setValue( "view_satellite" , m_viewSatellite ? 1 : 0 );
    settings.setValue( "view_borders"   , m_viewBorders   ? 1 : 0 );
    settings.setValue( "view_traces"    , m_viewTraces    ? 1 : 0 );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::actionViewCrops_toggled( bool checked )
{
    m_viewCrops = checked;
    cgi::Manager::instance()->setVisibilityCrops( checked );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::actionViewGrassland_toggled( bool checked )
{
    m_viewGrassland = checked;
    cgi::Manager::instance()->setVisibilityGrassland( checked );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::actionViewWoodland_toggled( bool checked )
{
    m_viewWoodland = checked;
    cgi::Manager::instance()->setVisibilityWoodland( checked );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::actionViewBuiltup_toggled( bool checked )
{
    m_viewBuiltup = checked;
    cgi::Manager::instance()->setVisibilityBuiltup( checked );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::actionViewRailroads_toggled( bool checked )
{
    m_viewRailroads = checked;
    cgi::Manager::instance()->setVisibilityRailroads( checked );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::actionViewRoads_toggled( bool checked )
{
    m_viewRoads = checked;
    cgi::Manager::instance()->setVisibilityRoads( checked );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::actionViewAirports_toggled( bool checked )
{
    m_viewAirports = checked;
    cgi::Manager::instance()->setVisibilityAirports( checked );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::actionViewSatellite_toggled( bool checked )
{
    m_viewSatellite = checked;
    cgi::Manager::instance()->setVisibilitySatellite( checked );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::actionViewBorders_toggled( bool checked )
{
    m_viewBorders = checked;
    cgi::Manager::instance()->setVisibilityBorders( checked );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::actionViewTraces_toggled( bool checked )
{
    m_viewTraces = checked;
    cgi::Manager::instance()->setVisibilityTraces( checked );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::actionTracesReset_triggered()
{
    cgi::Manager::instance()->resetTraces();
}
