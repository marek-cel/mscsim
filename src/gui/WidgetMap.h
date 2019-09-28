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
#ifndef WIDGETMAP_H
#define WIDGETMAP_H

////////////////////////////////////////////////////////////////////////////////

#include <QDateTime>
#include <QGridLayout>
#include <QWidget>

#include <osgViewer/Viewer>
#include <osgQt/GraphicsWindowQt>

#include <cgi/cgi_ManipulatorMap.h>

#include "gui_Defines.h"

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Map widget class.
 */
class WidgetMap : public QWidget, public osgViewer::Viewer
{
    Q_OBJECT

public:

    /** Constructor. */
    WidgetMap( QWidget *parent = NULLPTR );

    /** Destructor. */
    virtual ~WidgetMap();

signals:

    void mouseMoveGeoPosition( const QString & );

protected:

    /** */
    void contextMenuEvent( QContextMenuEvent *event );

    /** */
    bool event( QEvent *event );

    /** */
    void paintEvent( QPaintEvent *event );

    /** */
    void timerEvent( QTimerEvent *event );

private:

    QGridLayout *_gridLayout;

    osg::ref_ptr<osgQt::GraphicsWindowQt> _graphicsWindow;

    osg::ref_ptr<cgi::ManipulatorMap> _manipulator;

    QString _mouseGeoPositionStr;

    int _timerId;                   ///< timer ID

    bool _camManipulatorInited;

    bool _viewCrops;
    bool _viewGrassland;
    bool _viewWoodland;
    bool _viewBuiltup;
    bool _viewRailroads;
    bool _viewRoads;
    bool _viewAirports;
    bool _viewSatellite;
    bool _viewBorders;
    bool _viewTraces;

    /** */
    QWidget* addViewWidget();

    /** */
    void createCameraMap();

    /** */
    osg::ref_ptr<osgQt::GraphicsWindowQt> createGraphicsWindow( int x, int y, int w, int h,
                                                                const std::string &name = "",
                                                                bool windowDecoration = false );

    void updateMouseGeoPositionStr( double lat, double lon );

    void settingsRead();
    void settingsSave();

private slots:

    void actionViewCrops_toggled     ( bool checked );
    void actionViewGrassland_toggled ( bool checked );
    void actionViewWoodland_toggled  ( bool checked );
    void actionViewBuiltup_toggled   ( bool checked );
    void actionViewRailroads_toggled ( bool checked );
    void actionViewRoads_toggled     ( bool checked );
    void actionViewAirports_toggled  ( bool checked );
    void actionViewSatellite_toggled ( bool checked );
    void actionViewBorders_toggled   ( bool checked );
    void actionViewTraces_toggled    ( bool checked );

    void actionTracesReset_triggered();
};

////////////////////////////////////////////////////////////////////////////////

#endif // WIDGETMAP_H
