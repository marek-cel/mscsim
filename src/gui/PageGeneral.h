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
#ifndef PAGEGENERAL_H
#define PAGEGENERAL_H

////////////////////////////////////////////////////////////////////////////////

#include <QWidget>

#include "gui_Defines.h"

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class PageGeneral;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief General page class.
 */
class PageGeneral : public QWidget
{
    Q_OBJECT

public:

    /** Constructor. */
    explicit PageGeneral( QWidget *parent = NULLPTR );

    /** Destructor. */
    virtual ~PageGeneral();

    void readData();
    void saveData();

    inline const QColor& getHudColor() const { return _hud_color; }

    inline int getHudOpacity() const { return _hud_opacity; }

    inline double getHudFactorAlt() const { return _hud_factor_alt; }
    inline double getHudFactorVel() const { return _hud_factor_vel; }

    inline bool getWidescreen() const { return _widescreen; }

private:

    Ui::PageGeneral *_ui;       ///<

    QColor _hud_color;          ///<
    QColor _hud_color_temp;     ///<

    int _hud_opacity;           ///< [%]

    double _hud_factor_alt;     ///<
    double _hud_factor_vel;     ///<

    bool _widescreen;           ///< widescreen dock window layout

    void setFrameHudColor( const QColor &color );

    void settingsRead();
    void settingsSave();

private slots:

    void on_pushButtonHudColor_clicked();

    void on_comboBoxHudAltitude_currentIndexChanged( int index );
    void on_comboBoxHudVelocity_currentIndexChanged( int index );
};

////////////////////////////////////////////////////////////////////////////////

#endif // PAGEGENERAL_H
