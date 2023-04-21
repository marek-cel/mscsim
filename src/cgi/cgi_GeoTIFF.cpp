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

#include <cgi/cgi_GeoTIFF.h>

#ifdef WIN32
#   include <ogrsf_frmts.h>
#   include <ogr_spatialref.h>
#endif

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

GeoTIFF::GeoTIFF() {}

////////////////////////////////////////////////////////////////////////////////

GeoTIFF::GeoTIFF( const char *file )
{
    open( file );
}

////////////////////////////////////////////////////////////////////////////////

GeoTIFF::~GeoTIFF() {}

////////////////////////////////////////////////////////////////////////////////

void GeoTIFF::open( const char *file )
{
    close();

    GDALAllRegister();

    _dataset = static_cast< GDALDataset* >( GDALOpen( file, GA_ReadOnly ) );

    if ( _dataset )
    {
        _dataset->GetGeoTransform( _gt );

        _w_px = _dataset->GetRasterXSize();
        _h_px = _dataset->GetRasterYSize();
    }
}

////////////////////////////////////////////////////////////////////////////////

void GeoTIFF::close()
{
    if ( _dataset )
    {
        GDALClose( _dataset );
    }

    _dataset = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

bool GeoTIFF::compareSRS_PROJ4( const char *str ) const
{
    return ( 0 == strcmp( str, getSRS_PROJ4().c_str() ) );
}

////////////////////////////////////////////////////////////////////////////////

bool GeoTIFF::compareSRS_WKT( const char *str ) const
{
    return ( 0 == strcmp( str, getSRS_WKT().c_str() ) );
}

////////////////////////////////////////////////////////////////////////////////

bool GeoTIFF::isOpen() const
{
    return ( _dataset != nullptr );
}

////////////////////////////////////////////////////////////////////////////////

std::string GeoTIFF::getSRS_PROJ4() const
{
    std::string result;

    if ( _dataset )
    {
        const OGRSpatialReference *spatialRef = nullptr;

#       ifdef WIN32
        OGRLayer *layer = _dataset->GetLayer(0);
        if ( layer )
        {
            const OGRSpatialReference *spatialRef = layer->GetSpatialRef();
        }
#       else
        spatialRef = _dataset->GetSpatialRef();
#       endif

        if ( spatialRef )
        {
            char *str = nullptr;
            spatialRef->exportToProj4( &str );

            if ( str ) result = str;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

std::string GeoTIFF::getSRS_WKT() const
{
    std::string result;

    if ( _dataset )
    {
        const OGRSpatialReference *spatialRef = nullptr;

#       ifdef WIN32
        OGRLayer *layer = _dataset->GetLayer(0);
        if ( layer )
        {
            const OGRSpatialReference *spatialRef = layer->GetSpatialRef();
        }
#       else
        spatialRef = _dataset->GetSpatialRef();
#       endif

        if ( spatialRef )
        {
            char *str = nullptr;
            spatialRef->exportToWkt( &str );

            if ( str ) result = str;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

std::string GeoTIFF::getEPSG() const
{
    std::string result;

    if ( _dataset )
    {
        const OGRSpatialReference *spatialRef = nullptr;

#       ifdef WIN32
        OGRLayer *layer = _dataset->GetLayer(0);
        if ( layer )
        {
            const OGRSpatialReference *spatialRef = layer->GetSpatialRef();
        }
#       else
        spatialRef = _dataset->GetSpatialRef();
#       endif

        if ( spatialRef )
        {
            result = spatialRef->GetAttrValue( "PROJCS|GEOGCS|AUTHORITY", 1 );
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

double GeoTIFF::getMapX( double x_px, double y_ln ) const
{
    if ( _dataset )
    {
        return _gt[ 0 ] + x_px * _gt[ 1 ] + y_ln * _gt[ 2 ];
    }

    return std::numeric_limits< double >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

double GeoTIFF::getMapY( double x_px, double y_ln ) const
{
    if ( _dataset )
    {
        return _gt[ 3 ] + x_px * _gt[ 4 ] + y_ln * _gt[ 5 ];
    }

    return std::numeric_limits< double >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

double GeoTIFF::getMapUpperLeftX() const
{
    if ( _dataset )
    {
        return _gt[ 0 ];
    }

    return std::numeric_limits< double >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

double GeoTIFF::getMapUpperLeftY() const
{
    if ( _dataset )
    {
        return _gt[ 3 ];
    }

    return std::numeric_limits< double >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

double GeoTIFF::getMapW() const
{
    if ( _dataset )
    {
        return _w_px * _gt[ 1 ] + _h_px * _gt[ 2 ];
    }

    return std::numeric_limits< double >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

double GeoTIFF::getMapH() const
{
    if ( _dataset )
    {
        return _w_px * _gt[ 4 ] + _h_px * _gt[ 5 ];
    }

    return std::numeric_limits< double >::quiet_NaN();
}
