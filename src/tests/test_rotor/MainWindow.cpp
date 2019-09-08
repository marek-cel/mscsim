#include "MainWindow.h"
#include "ui_MainWindow.h"

////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow),
    _timerId ( 0 )
{
    _ui->setupUi(this);

    _timerId = startTimer( 16 );
}

////////////////////////////////////////////////////////////////////////////////

MainWindow::~MainWindow()
{
    killTimer( _timerId );

    if ( _ui ) delete _ui;
    _ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::timerEvent( QTimerEvent *event )
{
    /////////////////////////////////
    QMainWindow::timerEvent( event );
    /////////////////////////////////

    _ui->widgetCGI->update();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionMainRotor_triggered(bool checked)
{
    if ( checked )
    {
        _ui->actionPadfield->setChecked( false );
        _ui->actionBladeElement->setChecked( false );
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionPadfield_triggered(bool checked)
{
    if ( checked )
    {
        _ui->actionMainRotor->setChecked( false );
        _ui->actionBladeElement->setChecked( false );
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionBladeElement_triggered(bool checked)
{
    if ( checked )
    {
        _ui->actionMainRotor->setChecked( false );
        _ui->actionPadfield->setChecked( false );
    }
}
