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

#ifdef SIM_MARBLE_MAPS

////////////////////////////////////////////////////////////////////////////////

#include <gui/WidgetMarble.h>

#include <marble/MarbleDirs.h>
#include <marble/MarbleGlobal.h>
#include <marble/MarbleLocale.h>

//////////////

#include <marble/GeoDataDocument.h>
#include <marble/GeoDataGroundOverlay.h>
#include <marble/GeoDataTreeModel.h>
#include <marble/MarbleModel.h>

/////////////

#include <fdm/fdm_Path.h>

////////////////////////////////////////////////////////////////////////////////

WidgetMarble::WidgetMarble( QWidget *parent ) :
    Marble::MarbleWidget ( parent )
{
    QString dataPath = fdm::Path::get().c_str();

    if ( dataPath.length() == 0 )
    {
        dataPath = QDir::currentPath() + "/";
    }

    dataPath += "data/map/marble/";

    Marble::MarbleDirs::setMarbleDataPath( dataPath );

    setMapThemeId( "earth/mscsim/map_bmng.dgml" );
    //setMapThemeId( "earth/mscsim/map_osm.dgml" );

    //setProjection( Marble::Mercator );

    setShowTerrain( true );

    Marble::MarbleGlobal::getInstance()->locale()->setMeasurementSystem( Marble::MarbleLocale::NauticalSystem );

    //////////////////////////////////////


    // Create a bounding box from the given corner points
    Marble::GeoDataLatLonBox box( 55, 48, 14.5, 6, Marble::GeoDataCoordinates::Degree );
    box.setRotation( 0, Marble::GeoDataCoordinates::Degree );

    // Create an overlay and assign the image to render and its bounding box to it
    Marble::GeoDataGroundOverlay *overlay = new Marble::GeoDataGroundOverlay();
    overlay->setLatLonBox( box );
    overlay->setIcon( QImage( fdm::Path::get( "data/map/icons/air_friend.png" ).c_str() ) );

    // Create a document as a container for the overlay
    Marble::GeoDataDocument *document = new Marble::GeoDataDocument();
    document->append( overlay );

    // Add the document to MarbleWidget's tree model
    model()->treeModel()->addDocument( document );
}

////////////////////////////////////////////////////////////////////////////////

WidgetMarble::~WidgetMarble() {}

////////////////////////////////////////////////////////////////////////////////

bool WidgetMarble::event( QEvent *event )
{
    // first!
    //setHeading( 0.0 );

    ////////////////////////////////////////////
    return Marble::MarbleWidget::event( event );
    ////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

#endif // SIM_MARBLE_MAPS
