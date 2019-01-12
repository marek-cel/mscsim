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
#ifndef CGI_HUD_H
#define CGI_HUD_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/Geode>
#include <osg/PositionAttitudeTransform>
#include <osg/Switch>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief Generic HUD class.
 */
class HUD
{
public:

    static const float m_charSize;      ///<
    static const float m_angleCoef;     ///<
    static const float m_rollLimit;     ///<

    /** Constructor. */
    HUD();

    /** Destructor. */
    virtual ~HUD();

    /** Updates HUD. */
    void update();

    /** Returns OSG node.  */
    inline osg::Group* getNode() { return m_root.get(); }

private:

    osg::ref_ptr<osg::Group> m_root;        ///< HUD root group
    osg::ref_ptr<osg::Switch> m_switch;     ///<

    osg::ref_ptr<osg::Switch> m_switchRollIndicator;
    osg::ref_ptr<osg::Switch> m_switchPitchLadder;
    osg::ref_ptr<osg::Switch> m_switchHeadingScale;

    osg::ref_ptr<osg::Switch> m_switchFPM;
    osg::ref_ptr<osg::Switch> m_switchILS;

    osg::ref_ptr<osg::Switch> m_switchStall;

    osg::ref_ptr<osg::PositionAttitudeTransform> m_patPitchLadderRoll;
    osg::ref_ptr<osg::PositionAttitudeTransform> m_patPitchLadderPitch;
    osg::ref_ptr<osg::PositionAttitudeTransform> m_patPitchLadderWind;

    osg::ref_ptr<osg::PositionAttitudeTransform> m_patRollIndicator;
    osg::ref_ptr<osg::PositionAttitudeTransform> m_patHeadingScale;

    osg::ref_ptr<osg::PositionAttitudeTransform> m_patFPM;

    osg::ref_ptr<osg::PositionAttitudeTransform> m_patILS;
    osg::ref_ptr<osg::PositionAttitudeTransform> m_patILS_GS;
    osg::ref_ptr<osg::PositionAttitudeTransform> m_patILS_LC;

    osg::ref_ptr<osg::Geode> m_airspeed;
    osg::ref_ptr<osg::Geode> m_altitude;
    osg::ref_ptr<osg::Geode> m_machNo;
    osg::ref_ptr<osg::Geode> m_climbRate;
    osg::ref_ptr<osg::Geode> m_heading;
    osg::ref_ptr<osg::Geode> m_gForce;
    osg::ref_ptr<osg::Geode> m_radioAlt;

    float m_sideslipAngleFPM_deg;
    float m_angleOfAttackFPM_deg;

    bool m_validFPM;

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

    void createHeadingScaleBar( osg::Geode *geode, float y_del, int x, int deg10 );
    void createHeadingScaleBar( osg::Geode *geode, float y_del, int x );

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
