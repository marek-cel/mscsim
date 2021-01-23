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

#include <fdm_c172/c172_KAP140_AP.h>

#include <fdm/fdm_Path.h>

#include <fdm/xml/fdm_XmlDoc.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C172_KAP140_AP::C172_KAP140_AP() :
    Autopilot ( _fd = new C172_KAP140_FD( this ) )
{}

////////////////////////////////////////////////////////////////////////////////

C172_KAP140_AP::~C172_KAP140_AP()
{
    FDM_DELPTR( _fd );
}

////////////////////////////////////////////////////////////////////////////////

void C172_KAP140_AP::readData( XmlNode &dataNode )
{
    ////////////////////////////////
    Autopilot::readData( dataNode );
    ////////////////////////////////

    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        // TODO

        if ( result != FDM_SUCCESS )
        {
            XmlUtils::throwError( __FILE__, __LINE__, dataNode );
        }
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_KAP140_AP::initialize()
{
    std::string dataFile = Path::get( "fdm/c172/c172_ap_kap140.xml" );
    fdm::XmlDoc doc( dataFile.c_str() );

    if ( doc.isOpen() )
    {
        fdm::XmlNode rootNode = doc.getRootNode();

        if ( rootNode.isValid() )
        {
            readData( rootNode );
        }
    }
    else
    {
        Exception e;

        e.setType( Exception::FileReadingError );
        e.setInfo( "Reading file \"" + dataFile + "\" failed." );

        FDM_THROW( e );
    }

    ////////////////////////
    Autopilot::initialize();
    ////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void C172_KAP140_AP::update( double timeStep, bool button_dn, bool button_up )
{

}

////////////////////////////////////////////////////////////////////////////////

void C172_KAP140_AP::onPressedAP()
{
    _engaged = !_engaged;

    if ( _engaged ) _yaw_damper = true;
}

////////////////////////////////////////////////////////////////////////////////

void C172_KAP140_AP::onPressedFD()
{
    if ( _fd->isEngaged() )
    {
        _fd->disengage();

        if ( !_engaged )
        {
            _fd->setLatMode( C172_KAP140_FD::LM_ROL );
            _fd->setVerMode( C172_KAP140_FD::VM_PIT );
        }
    }
    else
    {
        _fd->engage();
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_KAP140_AP::onPressedALT()
{
    _fd->toggleVerMode( C172_KAP140_FD::VM_ALT );
}

////////////////////////////////////////////////////////////////////////////////

void C172_KAP140_AP::onPressedHDG()
{
    _fd->engage();
    _fd->toggleLatMode( C172_KAP140_FD::LM_HDG );
}

////////////////////////////////////////////////////////////////////////////////

void C172_KAP140_AP::onPressedNAV()
{

}

////////////////////////////////////////////////////////////////////////////////

void C172_KAP140_AP::onPressedAPR()
{

}
