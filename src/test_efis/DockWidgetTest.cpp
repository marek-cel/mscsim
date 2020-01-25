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

#include <test_efis/DockWidgetTest.h>
#include <ui_DockWidgetTest.h>

#include <iostream>
#include <math.h>

////////////////////////////////////////////////////////////////////////////////

DockWidgetTest::DockWidgetTest( QWidget *parent ) :
    QDockWidget ( parent ),
    _ui ( new Ui::DockWidgetTest ),

    _alpha     (  0.0 ),
    _beta      (  0.0 ),
    _roll      (  0.0 ),
    _pitch     (  0.0 ),
    _heading   (  0.0 ),
    _slipSkid  (  0.0 ),
    _turnRate  (  0.0 ),
    _course    (  0.0 ),
    _devH      (  0.0 ),
    _devV      (  0.0 ),
    _airspeed  (  0.0 ),
    _altitude  (  0.0 ),
    _pressure  ( 28.0 ),
    _climbRate (  0.0 ),
    _machNo    (  0.0 ),
    _adf       (  0.0 ),
    _dme       (  0.0 )
{
    _ui->setupUi( this );
}

////////////////////////////////////////////////////////////////////////////////

DockWidgetTest::~DockWidgetTest()
{
    DELPTR( _ui );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetTest::update( double realTime )
{
    //std::cout << realTime << std::endl;

    if ( _ui->pushButtonAuto->isChecked() )
    {
        _alpha     =   20.0 * sin( realTime /  10.0 );
        _beta      =   15.0 * sin( realTime /  10.0 );
        _roll      =  180.0 * sin( realTime /  10.0 );
        _pitch     =   90.0 * sin( realTime /  20.0 );
        _heading   =  360.0 * sin( realTime /  40.0 );
        _slipSkid  =    1.0 * sin( realTime /  10.0 );
        _turnRate  =    7.0 * sin( realTime /  10.0 );
        _course    = -360.0 * sin( realTime /  50.0 ) + 90.0;
        _devH      =    1.0 * sin( realTime /  20.0 );
        _devV      =    1.0 * sin( realTime /  20.0 );
        _airspeed  =  125.0 * sin( realTime /  40.0 ) +  125.0;
        _altitude  = 9000.0 * sin( realTime /  40.0 ) + 9000.0;
        _pressure  =    2.0 * sin( realTime /  20.0 ) +   30.0;
        _climbRate =  650.0 * sin( realTime /  20.0 );
        _machNo    = _airspeed / 650.0;
        _adf       = -360.0 * sin( realTime /  50.0 );
        _dme       =   99.0 * sin( realTime / 100.0 );

        _ui->spinBoxAlpha  ->setValue( _alpha     );
        _ui->spinBoxBeta   ->setValue( _beta      );
        _ui->spinBoxRoll   ->setValue( _roll      );
        _ui->spinBoxPitch  ->setValue( _pitch     );
        _ui->spinBoxSlip   ->setValue( _slipSkid  );
        _ui->spinBoxTurn   ->setValue( _turnRate  );
        _ui->spinBoxCourse ->setValue( _course    );
        _ui->spinBoxDevH   ->setValue( _devH      );
        _ui->spinBoxDevV   ->setValue( _devV      );
        _ui->spinBoxHead   ->setValue( _heading   );
        _ui->spinBoxSpeed  ->setValue( _airspeed  );
        _ui->spinBoxMach   ->setValue( _machNo    );
        _ui->spinBoxAlt    ->setValue( _altitude  );
        _ui->spinBoxPress  ->setValue( _pressure  );
        _ui->spinBoxClimb  ->setValue( _climbRate );
        _ui->spinBoxADF    ->setValue( _adf       );
        _ui->spinBoxDME    ->setValue( _dme       );
    }
    else
    {
        _alpha     = _ui->spinBoxAlpha  ->value();
        _beta      = _ui->spinBoxBeta   ->value();
        _roll      = _ui->spinBoxRoll   ->value();
        _pitch     = _ui->spinBoxPitch  ->value();
        _heading   = _ui->spinBoxHead   ->value();
        _slipSkid  = _ui->spinBoxSlip   ->value();
        _turnRate  = _ui->spinBoxTurn   ->value();
        _course    = _ui->spinBoxCourse ->value();
        _devH      = _ui->spinBoxDevH   ->value();
        _devV      = _ui->spinBoxDevV   ->value();
        _airspeed  = _ui->spinBoxSpeed  ->value();
        _pressure  = _ui->spinBoxPress  ->value();
        _altitude  = _ui->spinBoxAlt    ->value();
        _climbRate = _ui->spinBoxClimb  ->value();
        _machNo    = _ui->spinBoxMach   ->value();
        _adf       = _ui->spinBoxADF    ->value();
        _dme       = _ui->spinBoxDME    ->value();
    }
}
