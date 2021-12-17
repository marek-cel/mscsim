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
#ifndef CGI_GEOTIFF_H
#define CGI_GEOTIFF_H

////////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
#   include <gdal_priv.h>
#else
#   include <gdal/gdal_priv.h>
#endif

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief Georefernced TIFF info class.
 */
class GeoTIFF
{
public:

    /** @brief Constructor. */
    GeoTIFF();

    /**
     * @brief Constructor.
     * @param file dataset file name
     */
    GeoTIFF( const char *file );

    /** @brief Destructor. */
    virtual ~GeoTIFF();

    /**
     * @brief Opens dataset.
     * @param file dataset file name
     */
    void open( const char *file );

    /**
     * @brief Closes dataset.
     */
    void close();

    /**
     * @brief Compares spatial reference systems.
     * @param str spatial ref as PROJ.4 string
     * @return returns true if spatial reference systems are identical, otherwise returns false
     */
    bool compareSRS_PROJ4( const char *str ) const;

    /**
     * @brief Compares spatial reference systems.
     * @param str spatial ref as WKT string
     * @return returns true if spatial reference systems are identical, otherwise returns false
     */
    bool compareSRS_WKT( const char *str ) const;

    /**
     * @brief Checks if dataset is open.
     * @return returns true if dataset is open, otherwise returns false
     */
    bool isOpen() const;

    /**
     * @brief Gets spatial reference systems PROJ.4 string.
     * @return spatial reference systems PROJ.4 string
     */
    std::string getSRS_PROJ4() const;

    /**
     * @brief Gets spatial reference systems WKT string.
     * @return spatial reference systems WKT string
     */
    std::string getSRS_WKT() const;

    /** */
    std::string getEPSG() const;

    /**
     * @brief Returns x coordinate in map units.
     * @param x_px pixel
     * @param y_ln line
     * @return x coordinate in map units
     */
    double getMapX( double x_px, double y_ln ) const;

    /**
     * @brief Returns y coordinate in map units.
     * @param x_px pixel
     * @param y_ln line
     * @return y coordinate in map units
     */
    double getMapY( double x_px, double y_ln ) const;

    /**
     * @brief Returns raster upper left x coordinate in map units.
     * @return raster upper left x coordinate in map units
     */
    double getMapUpperLeftX() const;

    /**
     * @brief Returns raster upper left y coordinate in map units.
     * @return raster upper left y coordinate in map units
     */
    double getMapUpperLeftY() const;

    /**
     * @brief Returns raster width in map units.
     * @return raster width in map units
     */
    double getMapW() const;

    /**
     * @brief Returns raster height in map units.
     * @return raster height in map units
     */
    double getMapH() const;

private:

    GDALDataset *_dataset { nullptr };  ///< dataset

    double _gt[ 6 ] {};                 ///< geo transform

    double _w_px {};                    ///< [px] raster width
    double _h_px {};                    ///< [px] raster height
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_GEOTIFF_H
