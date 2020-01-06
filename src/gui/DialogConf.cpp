/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
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

#include <gui/DialogConf.h>
#include <ui_DialogConf.h>

#include <QSettings>

#include <gui/gui_Defines.h>

////////////////////////////////////////////////////////////////////////////////

DialogConf::DialogConf( QWidget *parent ) :
    QDialog ( parent ),
    _ui ( new Ui::DialogConf )
{
    _ui->setupUi( this );

    settingsRead();
}

////////////////////////////////////////////////////////////////////////////////

DialogConf::~DialogConf()
{
    settingsSave();
    
    DELPTR( _ui );
}

////////////////////////////////////////////////////////////////////////////////

void DialogConf::readData()
{
    _ui->pageGeneral->readData();
    _ui->pageControls->readData();
}

////////////////////////////////////////////////////////////////////////////////

void DialogConf::saveData()
{
    _ui->pageGeneral->saveData();
    _ui->pageControls->saveData();
}

////////////////////////////////////////////////////////////////////////////////

void DialogConf::updateAssignments()
{
    _ui->pageControls->updateAssignments();
}

////////////////////////////////////////////////////////////////////////////////

qreal DialogConf::getHudColorR() const
{
    return _ui->pageGeneral->getHudColor().redF();
}

////////////////////////////////////////////////////////////////////////////////

qreal DialogConf::getHudColorG() const
{
    return _ui->pageGeneral->getHudColor().greenF();
}

////////////////////////////////////////////////////////////////////////////////

qreal DialogConf::getHudColorB() const
{
    return _ui->pageGeneral->getHudColor().blueF();
}

////////////////////////////////////////////////////////////////////////////////

int DialogConf::getHudOpacity() const
{
    return _ui->pageGeneral->getHudOpacity();
}

////////////////////////////////////////////////////////////////////////////////

double DialogConf::getHudFactorAlt() const
{
    return _ui->pageGeneral->getHudFactorAlt();
}

////////////////////////////////////////////////////////////////////////////////

double DialogConf::getHudFactorVel() const
{
    return _ui->pageGeneral->getHudFactorVel();
}

////////////////////////////////////////////////////////////////////////////////

bool DialogConf::getWidescreen() const
{
    return _ui->pageGeneral->getWidescreen();
}

////////////////////////////////////////////////////////////////////////////////

void DialogConf::settingsRead()
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );
    
    settings.beginGroup( "dialog_conf" );

    restoreGeometry( settings.value( "geometry" ).toByteArray() );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogConf::settingsSave()
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );

    settings.beginGroup( "dialog_conf" );

    settings.setValue( "geometry" , saveGeometry() );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogConf::on_listMenu_currentRowChanged( int iRow )
{
    _ui->stackedContents->setCurrentIndex( iRow );
}
