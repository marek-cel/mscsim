#include "MainWindow.h"
#include "ui_MainWindow.h"

////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_timerId ( 0 )
{
    m_ui->setupUi(this);

    m_timerId = startTimer( 16 );
}

////////////////////////////////////////////////////////////////////////////////

MainWindow::~MainWindow()
{
    killTimer( m_timerId );

    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::timerEvent( QTimerEvent *event )
{
    /////////////////////////////////
    QMainWindow::timerEvent( event );
    /////////////////////////////////

    m_ui->widgetCGI->update();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionMainRotor_triggered(bool checked)
{
    if ( checked )
    {
        m_ui->actionPadfield->setChecked( false );
        m_ui->actionBladeElement->setChecked( false );
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionPadfield_triggered(bool checked)
{
    if ( checked )
    {
        m_ui->actionMainRotor->setChecked( false );
        m_ui->actionBladeElement->setChecked( false );
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionBladeElement_triggered(bool checked)
{
    if ( checked )
    {
        m_ui->actionMainRotor->setChecked( false );
        m_ui->actionPadfield->setChecked( false );
    }
}
