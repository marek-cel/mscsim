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

#include <gui/DockWidgetEFIS.h>
#include <ui_DockWidgetEFIS.h>

////////////////////////////////////////////////////////////////////////////////

DockWidgetEFIS::DockWidgetEFIS( QWidget *parent ) :
    QDockWidget ( parent ),
    m_ui ( new Ui::DockWidgetEFIS ),
    m_graphicsEADI ( 0 ),
    m_graphicsEHSI ( 0 )
{
    m_ui->setupUi( this );

    m_graphicsEADI = m_ui->graphicsEADI;
    m_graphicsEHSI = m_ui->graphicsEHSI;

    m_ui->widgetEADI->setSquareWidget( m_ui->frameEADI );
    m_ui->widgetEHSI->setSquareWidget( m_ui->frameEHSI );

    m_graphicsEHSI->setBearing( 0.0f, false );
    m_graphicsEHSI->setDeviation( 0.0f, false );
    m_graphicsEHSI->setDistance( 0.0f, false );
}

////////////////////////////////////////////////////////////////////////////////

DockWidgetEFIS::~DockWidgetEFIS()
{
    if ( m_ui ) delete m_ui;
    m_ui = 0;
}
