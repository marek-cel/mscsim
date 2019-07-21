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
#ifndef CGI_MAP_H
#define CGI_MAP_H

////////////////////////////////////////////////////////////////////////////////

#include <cgi/cgi_Module.h>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief Map scene root class.
 */
class Map : public Module
{
public:

    static const osg::Vec3 colorOceans;
    static const osg::Vec3 colorLandmass;
    static const osg::Vec3 colorWoodland;
    static const osg::Vec3 colorBuiltup;
    static const osg::Vec3 colorRailroads;
    static const osg::Vec3 colorRoads;
    static const osg::Vec3 colorAirports;
    static const osg::Vec3 colorWater;

    static const double zOceans;
    static const double zLandmass;
    static const double zWoodland;
    static const double zBuiltup;
    static const double zRailroads;
    static const double zRoads;
    static const double zAirports;
    static const double zWater;

    static const double zSpeedLeader;
    static const double zIconsFill;

    /** Constructor. */
    Map( Module *parent = 0 );

    /** Destructor. */
    virtual ~Map();

    /** */
    void setScale( double scale );
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_MAP_H
