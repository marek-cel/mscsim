#ifndef MAINWINDOW_H
#define MAINWINDOW_H

////////////////////////////////////////////////////////////////////////////////

#include <QMainWindow>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class MainWindow;
}

////////////////////////////////////////////////////////////////////////////////

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

protected:

    /** */
    void timerEvent( QTimerEvent *event );

private slots:

    void on_actionMainRotor_triggered(bool checked);
    void on_actionPadfield_triggered(bool checked);
    void on_actionBladeElement_triggered(bool checked);

private:

    Ui::MainWindow *_ui;

    int _timerId;               ///<
};

////////////////////////////////////////////////////////////////////////////////

#endif // MAINWINDOW_H
