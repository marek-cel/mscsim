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
#ifndef CGI_ICONS_H
#define CGI_ICONS_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/PositionAttitudeTransform>

#include <cgi/cgi_Module.h>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/** Icons class. */
class Icons : public Module
{
public:

    struct Ownship
    {
        osg::ref_ptr<osg::PositionAttitudeTransform> pat;
        osg::ref_ptr<osg::Group> speedLeader;
    };

    typedef std::vector< osg::ref_ptr<osg::PositionAttitudeTransform> > PATs;

    /** @brief Constructor. */
    Icons( const Module *parent = NULLPTR );

    /** @brief Destructor. */
    virtual ~Icons();

    /** */
    void update();

    /** */
    void setScale( double scale );

private:

    Ownship _ownship;

    PATs _aerodromes;
    PATs _navaids;
    PATs _localizers;
    PATs _runways;

    osg::ref_ptr<osg::Node> _symbolAerodrome;
    osg::ref_ptr<osg::Node> _symbolNavaidVOR;
    osg::ref_ptr<osg::Node> _symbolNavaidVORTAC;
    osg::ref_ptr<osg::Node> _symbolNavaidVORDME;
    osg::ref_ptr<osg::Node> _symbolNavaidILSLOC;

    void createIcon( osg::Group *parent, float z, const char *textureFile );

    void createOwnship();

    osg::Node* createRunway( double length, double width );
    osg::Node* createSymbolILS();

    osg::Node* getNavaidSymbol( int type );

    void initAerodromes();
    void initLocalizers();
    void initNavaids();
    void initRunways();

    void updateOwnship();
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_ICONS_H
