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
#ifndef CGI_HUD_H
#define CGI_HUD_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/Geode>
#include <osg/Material>
#include <osg/PositionAttitudeTransform>
#include <osg/Switch>

#include <osgText/Text>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief Generic HUD class.
 */
class HUD
{
public:

    static const double _charSize;      ///<
    static const double _angleCoef;     ///<
    static const double _rollLimit;     ///<

    /** @brief Constructor. */
    HUD();

    /** @brief Destructor. */
    virtual ~HUD();

    /** @brief Updates HUD. */
    void update();

    /** @brief Returns OSG node.  */
    inline osg::Group* getNode() { return _root.get(); }

private:

    osg::ref_ptr<osg::Group>  _root;        ///< HUD root group
    osg::ref_ptr<osg::Switch> _switch;      ///<

    osg::ref_ptr<osg::Switch> _switchRollIndicator;
    osg::ref_ptr<osg::Switch> _switchPitchLadder;
    osg::ref_ptr<osg::Switch> _switchHeadingScale;

    osg::ref_ptr<osg::Switch> _switchFPM;
    osg::ref_ptr<osg::Switch> _switchILS;

    osg::ref_ptr<osg::Switch> _switchStall;

    osg::ref_ptr<osg::PositionAttitudeTransform> _patPitchLadderRoll;
    osg::ref_ptr<osg::PositionAttitudeTransform> _patPitchLadderPitch;
    osg::ref_ptr<osg::PositionAttitudeTransform> _patPitchLadderSlip;

    osg::ref_ptr<osg::PositionAttitudeTransform> _patRollIndicator;
    osg::ref_ptr<osg::PositionAttitudeTransform> _patHeadingScale;

    osg::ref_ptr<osg::PositionAttitudeTransform> _patFPM;

    osg::ref_ptr<osg::PositionAttitudeTransform> _patILS;
    osg::ref_ptr<osg::PositionAttitudeTransform> _patILS_GS;
    osg::ref_ptr<osg::PositionAttitudeTransform> _patILS_LC;

    osg::ref_ptr<osg::Geode> _airspeed;
    osg::ref_ptr<osg::Geode> _altitude;
    osg::ref_ptr<osg::Geode> _machNo;
    osg::ref_ptr<osg::Geode> _climbRate;
    osg::ref_ptr<osg::Geode> _heading;
    osg::ref_ptr<osg::Geode> _gForce;
    osg::ref_ptr<osg::Geode> _radioAlt;

    osg::ref_ptr<osg::Material> _material;

    std::vector< osg::ref_ptr<osgText::Text> > _texts;

    osg::Vec4 _color;

    double _sideslipAngleFPM_deg;
    double _angleOfAttackFPM_deg;

    bool _validFPM;

    void createWaterLine();

    void createRollIndicator();
    void createPitchLadder();
    void createHeadingScale();

    void createFPM();
    void createILS();

    void createStall();

    void createTextL();
    void createTextR();

    void createPitchLadderBar( osg::Geode *geode, int y, int deg );

    void createHeadingScaleBar( osg::Geode *geode, double y_del, int x, int deg10 );
    void createHeadingScaleBar( osg::Geode *geode, double y_del, int x );

    void updateRollIndicator();
    void updatePitchLadder();
    void updateHeadingScale();

    void updateFPM();
    void updateILS();

    void updateStall();

    void updateTextL();
    void updateTextR();
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_HUD_H
