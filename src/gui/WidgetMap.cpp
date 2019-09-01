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
    _gridLayout ( 0 ),
    _timerId ( 0 ),
    _camManipulatorInited ( false ),
    _viewCrops     ( false ),
    _viewGrassland ( false ),
    _viewWoodland  ( false ),
    _viewBuiltup   ( false ),
    _viewRailroads ( false ),
    _viewRoads     ( false ),
    _viewAirports  ( true  ),
    _viewSatellite ( false ),
    _viewBorders   ( false ),
    _viewTraces    ( true  )
{
    setMouseTracking( true );

    setThreadingModel( osgViewer::ViewerBase::SingleThreaded );
    //setThreadingModel( osgViewer::ViewerBase::ThreadPerContext );

    _graphicsWindow = createGraphicsWindow( x(), y(), width(), height() );

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

    QWidget *widget = addViewWidget();

    _gridLayout = new QGridLayout( this );
    _gridLayout->setContentsMargins( 1, 1, 1, 1 );
    _gridLayout->addWidget( widget, 0, 0 );

    setLayout( _gridLayout );

    settingsRead();

    cgi::Manager::instance()->setVisibilityCrops     ( _viewCrops     );
    cgi::Manager::instance()->setVisibilityGrassland ( _viewGrassland );
    cgi::Manager::instance()->setVisibilityWoodland  ( _viewWoodland  );
    cgi::Manager::instance()->setVisibilityBuiltup   ( _viewBuiltup   );
    cgi::Manager::instance()->setVisibilityRailroads ( _viewRailroads );
    cgi::Manager::instance()->setVisibilityRoads     ( _viewRoads     );
    cgi::Manager::instance()->setVisibilityAirports  ( _viewAirports  );
    cgi::Manager::instance()->setVisibilitySatellite ( _viewSatellite );
    cgi::Manager::instance()->setVisibilityBorders   ( _viewBorders   );
    cgi::Manager::instance()->setVisibilityTraces    ( _viewTraces    );

    _timerId = startTimer( GUI_TIME_STEP );
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

    actionViewCrops     .setChecked( _viewCrops     );
    actionViewGrassland .setChecked( _viewGrassland );
    actionViewWoodland  .setChecked( _viewWoodland  );
    actionViewBuiltup   .setChecked( _viewBuiltup   );
    actionViewRailroads .setChecked( _viewRailroads );
    actionViewRoads     .setChecked( _viewRoads     );
    actionViewAirports  .setChecked( _viewAirports  );
    actionViewSatellite .setChecked( _viewSatellite );
    actionViewBorders   .setChecked( _viewBorders   );
    actionViewTraces    .setChecked( _viewTraces    );

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

    updateMouseGeoPositionStr( _manipulator->getMouseLat(),
                               _manipulator->getMouseLon() );

    emit mouseMoveGeoPosition( _mouseGeoPositionStr );
}

////////////////////////////////////////////////////////////////////////////////

QWidget* WidgetMap::addViewWidget()
{
    createCameraMap();

    setSceneData( cgi::Manager::instance()->getNodeMap() );
    addEventHandler( new osgViewer::StatsHandler );
    setCameraManipulator( _manipulator.get() );

    setKeyEventSetsDone( 0 );

    assignSceneDataToCameras();

    return _graphicsWindow->getGLWidget();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::createCameraMap()
{
    osg::ref_ptr<osg::Camera> cameraMap = getCamera();

    cameraMap->setGraphicsContext( _graphicsWindow );

    const osg::GraphicsContext::Traits *traits = _graphicsWindow->getTraits();

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

    _mouseGeoPositionStr.clear();

    _mouseGeoPositionStr += QString("%1").arg( lon_d, 3, 'f', 0, QChar(' ') );
    _mouseGeoPositionStr += QString::fromUtf8( "° " );
    _mouseGeoPositionStr += QString("%1").arg( lon_m, 2, 'f', 0, QChar('0') );
    _mouseGeoPositionStr += "' ";
    _mouseGeoPositionStr += QString("%1").arg( lon_s, 5, 'f', 2, QChar('0') );
    _mouseGeoPositionStr += "\"";
    _mouseGeoPositionStr += ( lon > 0.0 ) ? "E" : "W";

    _mouseGeoPositionStr += ",  ";

    _mouseGeoPositionStr += QString("%1").arg( lat_d, 2, 'f', 0, QChar(' ') );
    _mouseGeoPositionStr += QString::fromUtf8( "° " );
    _mouseGeoPositionStr += QString("%1").arg( lat_m, 2, 'f', 0, QChar('0') );
    _mouseGeoPositionStr += "' ";
    _mouseGeoPositionStr += QString("%1").arg( lat_s, 5, 'f', 2, QChar('0') );
    _mouseGeoPositionStr += "\"";
    _mouseGeoPositionStr += ( lat > 0.0 ) ? "N" : "S";
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::settingsRead()
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );

    settings.beginGroup( "widget_map" );

    _viewCrops     = settings.value( "view_crops"     , _viewCrops     ).toBool();
    _viewGrassland = settings.value( "view_grassland" , _viewGrassland ).toBool();
    _viewWoodland  = settings.value( "view_woodland"  , _viewWoodland  ).toBool();
    _viewBuiltup   = settings.value( "view_builtup"   , _viewBuiltup   ).toBool();
    _viewRailroads = settings.value( "view_railroads" , _viewRailroads ).toBool();
    _viewRoads     = settings.value( "view_roads"     , _viewRoads     ).toBool();
    _viewAirports  = settings.value( "view_airports"  , _viewAirports  ).toBool();
    _viewSatellite = settings.value( "view_satellite" , _viewSatellite ).toBool();
    _viewBorders   = settings.value( "view_borders"   , _viewBorders   ).toBool();
    _viewTraces    = settings.value( "view_traces"    , _viewTraces    ).toBool();

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::settingsSave()
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );

    settings.beginGroup( "widget_map" );

    settings.setValue( "view_crops"     , _viewCrops     ? 1 : 0 );
    settings.setValue( "view_grassland" , _viewGrassland ? 1 : 0 );
    settings.setValue( "view_woodland"  , _viewWoodland  ? 1 : 0 );
    settings.setValue( "view_builtup"   , _viewBuiltup   ? 1 : 0 );
    settings.setValue( "view_railroads" , _viewRailroads ? 1 : 0 );
    settings.setValue( "view_roads"     , _viewRoads     ? 1 : 0 );
    settings.setValue( "view_airports"  , _viewAirports  ? 1 : 0 );
    settings.setValue( "view_satellite" , _viewSatellite ? 1 : 0 );
    settings.setValue( "view_borders"   , _viewBorders   ? 1 : 0 );
    settings.setValue( "view_traces"    , _viewTraces    ? 1 : 0 );

    settings.endGroup();
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

void WidgetMap::actionViewAirports_toggled( bool checked )
{
    _viewAirports = checked;
    cgi::Manager::instance()->setVisibilityAirports( checked );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::actionViewSatellite_toggled( bool checked )
{
    _viewSatellite = checked;
    cgi::Manager::instance()->setVisibilitySatellite( checked );
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
