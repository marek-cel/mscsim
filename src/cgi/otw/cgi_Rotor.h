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
#ifndef CGI_ROTOR_H
#define CGI_ROTOR_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/PositionAttitudeTransform>
#include <osg/Switch>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief Helicopter rotor class.
 */
class Rotor
{
public:

    typedef std::vector< osg::ref_ptr<osg::Vec3Array> > Paths;

    /** @brief Constructor. */
    Rotor( const osg::Vec3 &center, int blades_count,
           double radius, double offset, double inclination );

    /** @brief Destructor. */
    virtual ~Rotor();

    /** @brief Updates rotor. */
    void update();

    /** @brief Returns root node.  */
    inline osg::Group* getNode() { return _root.get(); }

private:

    const osg::Vec3 _center;        ///< [m] rotor hub center

    const int _blades_count;        ///< number of rotor blades

    const double _radius;           ///< [m] rotor radius
    const double _offset;           ///< [m] flapping hinge offset from rotor shaft
    const double _inclination;      ///< [rad] rotor inclination angle

    osg::ref_ptr<osg::Group> _root;

    osg::ref_ptr<osg::PositionAttitudeTransform> _pat;
    osg::ref_ptr<osg::PositionAttitudeTransform> _patDatum;

    osg::ref_ptr<osg::Switch> _switchBlur;
    osg::ref_ptr<osg::Switch> _switchDatum;
    osg::ref_ptr<osg::Switch> _switchPaths;
    osg::ref_ptr<osg::Switch> _switchVectors;

    Paths _paths;

    void createDatum();

    void updateBlur();

    void updateDatum();

    void updatePaths();
    void updatePath( osg::Group *parent, osg::Vec3Array *positions );

    void updateVectors();
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_ROTOR_H
