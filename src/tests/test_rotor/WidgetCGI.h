#ifndef WIDGETCGI_H
#define WIDGETCGI_H

////////////////////////////////////////////////////////////////////////////////

#include <osgGA/NodeTrackerManipulator>
#include <osgQt/GraphicsWindowQt>
#include <osgViewer/Viewer>

#include <QDateTime>
#include <QGridLayout>
#include <QWidget>

#include "SceneRoot.h"

////////////////////////////////////////////////////////////////////////////////

/** This is widget wrapper for CGI. */
class WidgetCGI : public QWidget, public osgViewer::Viewer
{
    Q_OBJECT

public:

    /** Constructor. */
    WidgetCGI( QWidget *parent = 0 );

    /** Destructor. */
    virtual ~WidgetCGI();

    /** */
    void update();

protected:

    /** */
    void paintEvent( QPaintEvent *event );

private:

    SceneRoot   *_sceneRoot;
    QGridLayout *_gridLayout;

    osg::ref_ptr<osgGA::NodeTrackerManipulator> _manipulator;

    osg::ref_ptr<osgQt::GraphicsWindowQt> _graphicsWindow;

    /** */
    QWidget* addViewWidget( osgQt::GraphicsWindowQt *graphicsWindow, osg::Node *scene );

    /** */
    osg::ref_ptr<osgQt::GraphicsWindowQt> createGraphicsWindow( int x, int y, int w, int h, const std::string &name = "", bool windowDecoration = false );
};

////////////////////////////////////////////////////////////////////////////////

#endif // WIDGETCGI_H
