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
#ifndef DIALOGACTION_H
#define DIALOGACTION_H

////////////////////////////////////////////////////////////////////////////////

#include <QDialog>

#include <hid/hid_Manager.h>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class DialogAction;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Action dialog class.
 */
class DialogAction : public QDialog
{
    Q_OBJECT

public:

    typedef hid::Assignment Assignment;
    typedef hid::Assignment::POVs POVs;

    static Assignment getJoyAxisAssignment( QWidget *parent = 0, Assignment assignment = Assignment(), short joystickId = 0 );
    static Assignment getJoyButtAssignment( QWidget *parent = 0, Assignment assignment = Assignment(), short joystickId = 0 );
    static Assignment getKeyAssignment( QWidget *parent = 0, Assignment assignment = Assignment() );

    explicit DialogAction( QWidget *parent = 0, short joystickId = -1 );

    ~DialogAction();

protected:

    void timerEvent( QTimerEvent *event );

private:

    Ui::DialogAction *m_ui;

    int m_timerId;

    short m_keyId;

    short m_axisCount;
    short m_buttCount;
    short m_povsCount;

    short m_joystickId;
    short m_axisId;
    short m_buttonId;
    short m_povId;
    POVs  m_povDir;
    bool  m_inverted;

private slots:

    void on_comboAxis_currentIndexChanged( int index );
    void on_comboButt_currentIndexChanged( int index );
    void on_comboKeys_currentIndexChanged( int index );
    void on_checkAxisInverted_toggled( bool checked );
};

////////////////////////////////////////////////////////////////////////////////

#endif // DIALOGACTION_H
