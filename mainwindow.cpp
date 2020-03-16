#include <QDebug>
#include <QLabel>
#include <QComboBox>
#include <QThread>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "plot.h"
#include "cdatabufffer.h"

#include "cqserialinterface.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_portname = "ttyACM0";

    m_ctrl = new CCS5451_controller;
    m_iface = new CQSerialInterface;

    CQSerialInterface *s_iface = dynamic_cast<CQSerialInterface*>(m_iface);

    s_iface->set_port_name(m_portname);
    m_ctrl->set_interface(s_iface);

    d_plot_voltages = new Plot(this,
                               m_ctrl->get_curve_data(0),
                               m_ctrl->get_curve_data(2),
                               m_ctrl->get_curve_data(4));


#if 0
   m_statusLeft = new QLabel("Left", this);
   m_statusLeft->setFrameStyle(QFrame::Panel | QFrame::Sunken);

   m_statusMiddle = new QLabel("", this);
   m_statusMiddle->setFrameStyle(QFrame::Panel | QFrame::Sunken);
   m_statusRight = new QLabel(m_portname, this);
   m_statusRight->setFrameStyle(QFrame::Panel | QFrame::Sunken);
   statusBar()->addPermanentWidget(m_statusLeft, 2);
   statusBar()->addPermanentWidget(m_statusMiddle, 1);
   statusBar()->addPermanentWidget(m_statusRight, 0);
#endif


    if( m_iface->open())
    {
        QString msg = "Serial port opened";
        ui->statusBar->showMessage(msg, 4000);
    }
    else {
        QString msg = QString("Serial port not opened");
        ui->statusBar->showMessage(msg);
    }

    ui->verticalLayout->addWidget(d_plot_voltages,10);
   // d_plot_voltages->setTitle("Voltages");
    //d_plot_voltages->replot();

    d_plot_currents = new Plot(this,
                               m_ctrl->get_curve_data(1),
                               m_ctrl->get_curve_data(3),
                               m_ctrl->get_curve_data(5));
    ui->verticalLayout->addWidget(d_plot_currents, 10);
    //d_plot_currents->setTitle("Currents");
    //d_plot_currents->replot();

    connect(ui->menuSerialPort->actions().first(),
            &QAction::triggered,
            this,
            &MainWindow::actionCmdStart);
    {
    auto actions = ui->menuFile->actions();
    connect(actions.first(), &QAction::triggered, this, &MainWindow::close);
    }

    {
        auto actions = ui->menuStart->actions();
        connect(actions.first(),
                &QAction::triggered,
                this,
                &MainWindow::actionStart);
    }

    {
        auto actions = ui->menuStop->actions();
        connect(actions.first(),
                &QAction::triggered,
                this,
                &MainWindow::actionStop);
    }

    createActions();

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<2; j++)
        {
            ui->tblVoltages->setItem(i,j, new QTableWidgetItem);
            ui->tblCurrents->setItem(i,j,new QTableWidgetItem);

            ui->tblVoltages->item(i,j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->tblCurrents->item(i,j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->tblVoltages->setColumnWidth(j,70);
            ui->tblCurrents->setColumnWidth(j,70);
        }
    }
}

void MainWindow::createActions()
{
    actGain = new QAction("Gain", this);
    actGain->setCheckable(true);
    actGain->setToolTip("40 times current amplification");
    connect( actGain, &QAction::triggered, this, &MainWindow::actionGain);
    ui->menuSerialPort->addAction(actGain);

    actOwrs = new QAction("Owrs", this);
    actOwrs->setCheckable(true);
    actOwrs->setToolTip("set 4 kHz convert frequency");
    connect(actOwrs, &QAction::triggered, this, &MainWindow::actionOwrs);
    ui->menuSerialPort->addAction(actOwrs);


}

MainWindow::~MainWindow()
{
    delete m_ctrl;
    delete ui;
}

void MainWindow::actionStart()
{
    m_timer = startTimer(100);
}

void MainWindow::actionStop()
{
    killTimer(m_timer);
}

void MainWindow::actionCmdStart()
{
    if( m_iface->isOpen() )
    {

        m_ctrl->start();
    }
    else
        qDebug() << __FUNCTION__ << "port not opened";
}

void MainWindow::actionGain(bool checked)
{
    m_ctrl->set_gain(checked);
}

void MainWindow::actionOwrs(bool checked)
{
    m_ctrl->set_owrs(checked);
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if( event->timerId() == m_timer)
    {
        update_graphs();
    }
}

void MainWindow::update_graphs()
{
    m_ctrl->prepare();
    d_plot_currents->replot();
    d_plot_voltages->replot();

    auto rms = m_ctrl->rms();

    for(int i=0; i<3; i++)
    {
        ui->tblVoltages->item(i,0)->setText(QString("%1").arg(rms[i*2]));
        ui->tblCurrents->item(i,0)->setText(QString("%1").arg(rms[i*2+1]));

        ui->tblVoltages->item(i,1)->setText(QString("%1").arg(rms[i*2]*0.0732f*3.0f));
        ui->tblCurrents->item(i,1)->setText(QString("%1").arg(rms[i*2+1]*0.15f));
    }
}
