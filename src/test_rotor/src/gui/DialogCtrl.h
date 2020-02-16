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

#ifndef DIALOGCTRL_H
#define DIALOGCTRL_H

////////////////////////////////////////////////////////////////////////////////

#include <QDialog>
#include <QSettings>

#include <defs.h>

#include <hid/hid_Assignment.h>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class DialogCtrl;
}

////////////////////////////////////////////////////////////////////////////////

/** This is 'Preferences' dialog class. */
class DialogCtrl : public QDialog
{
    Q_OBJECT

public:

    /** Constructor. */
    explicit DialogCtrl( QWidget *parent = 0 );
    
    /** Destructor. */
    virtual ~DialogCtrl();

    /** */
    void getAssignments( hid::Assignment assignments[] );

    void readData();
    void saveData();

    void updateAssignments();
    
private:

    Ui::DialogCtrl *_ui;

    bool _defaultAssignmentsInited;

    bool _comboDeviceInited;

    std::string     _actionNames[ HID_MAX_ACTIONS ];
    hid::Assignment _assignments[ HID_MAX_ACTIONS ];

    bool isButtonAssignmentClearEnabled( int action );
    bool isButtonAssignmentChangeEnabled( int action, int device );

    void changeAssignment( int action );
    void clearAssignment( int action );

    void createTableControls();
    void updateTableControls();

    void setButtonsState( int action, int device );

    void settingsRead();
    void settingsSave();

    void initDefaultAssignments();

    void assignmentsRead( QSettings &settings );
    void assignmentsSave( QSettings &settings );

    void assignmentRead( QSettings &settings, hid::Assignment &assignment, hid::Assignment::Action action );
    void assignmentSave( QSettings &settings, hid::Assignment &assignment, hid::Assignment::Action action );

private slots:

    void on_buttonAssignmentClear_clicked();
    void on_buttonAssignmentChange_clicked();
    void on_comboDevice_currentIndexChanged( int index );
    void on_tableControls_cellDoubleClicked( int row, int );
    void on_tableControls_currentCellChanged( int row, int, int, int );
};

////////////////////////////////////////////////////////////////////////////////

#endif // DIALOGCTRL_H
