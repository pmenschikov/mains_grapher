#include <QDebug>
#include <QLabel>
#include <QComboBox>
#include <QThread>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <complex>

#include "ccalibrationwidget.h"

#include "plot.h"
#include "cdatabufffer.h"

#include "cqserialinterface.h"

#include <fftw3.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
   // ,ui_power(new powers(this))
{
    ui->setupUi(this);
    ui_power1 = new powers_widget();
    ui_vectors = new VectorDiagramWidget;

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


    if( m_iface->open())
    {
        QString msg = "Serial port opened";
        ui->statusBar->showMessage(msg);
    }
    else {
        QString msg = QString("Serial port not opened");
        ui->statusBar->showMessage(msg);
    }

    ui->verticalLayout->addWidget(d_plot_voltages,10);
    d_plot_voltages->setMinimumSize(444,100);

    d_plot_currents = new Plot(this,
                               m_ctrl->get_curve_data(1),
                               m_ctrl->get_curve_data(3),
                               m_ctrl->get_curve_data(5));
    ui->verticalLayout->addWidget(d_plot_currents, 10);
    d_plot_currents->setMinimumSize(444,100);


    connect((CQSerialInterface*)m_iface,&CQSerialInterface::sig_error, this,
            &MainWindow::if_error);
    connect(ui->menuPort->actions().first(),&QAction::triggered,this,
            &MainWindow::port_open);
    connect(ui->menuPort->actions().last(), &QAction::triggered, this,
            &MainWindow::port_close);
    connect(ui->menuMeasuremens->actions().first(), &QAction::triggered,
            this,&MainWindow::ui_power);
    connect(ui->menuMeasuremens->actions().last(), &QAction::triggered,
            this,&MainWindow::ui_vectors_show);
    {
    auto actions = ui->menuFile->actions();
    connect(actions.first(), &QAction::triggered, this, &MainWindow::close);
    }
    {
      auto actions = ui->menuSerialPort->actions();
    connect(actions.first(),
            &QAction::triggered,
            this,
            &MainWindow::actionCmdStart);
    connect(actions.at(1), &QAction::triggered, this, &MainWindow::adc_calibrate);
    connect(actions.at(2),&QAction::triggered, this, &MainWindow::calibration_dialog);
    }
    connect(ui->menuStart,&QMenu::aboutToShow, this, &MainWindow::actionStart);
    connect(ui->menuStop, &QMenu::aboutToShow, this, &MainWindow::actionStop);

    createActions();

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<2; j++)
        {
            ui->tblVoltages->setItem(i,j, new QTableWidgetItem);
            ui->tblCurrents->setItem(i,j,new QTableWidgetItem);
            ui->tblPowers->setItem(i,j, new QTableWidgetItem);

            ui->tblVoltages->item(i,j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->tblCurrents->item(i,j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            //ui->tblVoltages->setColumnWidth(j,70);
            //ui->tblCurrents->setColumnWidth(j,70);
        }
    }

    connect(this, &MainWindow::update_powers, ui_power1, &powers_widget::set_powers);

    connect(this, &MainWindow::update_fft, ui_vectors, &VectorDiagramWidget::update_fft);

    m_fft.prepare_plans();
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

void MainWindow::ui_power()
{
  ui_power1->show();
}

void MainWindow::ui_vectors_show()
{
  ui_vectors->show();
}

void MainWindow::calibration_dialog()
{
  CCalibrationWidget widget;
  CCalibrationData data;
  widget.setModal(true);
  widget.set_calibration(m_ctrl->get_calibration());
  if( widget.exec() == QDialog::Accepted && widget.get_calibration(data))
    {
      m_ctrl->set_calibration(data);
    }
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

void MainWindow::adc_calibrate()
{
  qDebug() << m_ctrl->zero();
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

    CMeasurements* measurements = m_ctrl->measurements();

    for(int i=0; i<3; i++)
    {
        ui->tblVoltages->item(i,1)->
            setText(QString("%1").arg(m_ctrl->rms_voltage(i*2),0,'f',2));
        ui->tblCurrents->item(i,1)->
            setText(QString("%1").arg(m_ctrl->rms_current(i*2+1),0,'f',2));

        ui->tblVoltages->item(i,0)->setText(QString("%1").
                                            arg(measurements->m_rms[i*2],0,'f',2));
        ui->tblCurrents->item(i,0)->setText(QString("%1").
                                            arg(measurements->m_rms[i*2+1],0,'f',2));

        ui->tblPowers->item(i,0)->setText(
              QString("%1").arg(m_ctrl->s_power(i),0,'f',2));
        ui->tblPowers->item(i,1)->setText(
              QString("%1").arg(m_ctrl->p_power(i),0,'f',2));
    }

    emit update_powers(m_ctrl);
    const int N = FFT_SIZE;

    for(int i=0; i<N; i++)
      {
        m_fft.fft_inA[i] = static_cast<double>(m_ctrl->get_value(i, 0));
        m_fft.fft_inB[i] = static_cast<double>(m_ctrl->get_value(i, 2));
        m_fft.fft_inC[i] = static_cast<double>(m_ctrl->get_value(i, 4));
       // qDebug() << fft_inA[i] << fft_inB[i] << fft_inC[i];
      }
    m_fft.execute();

//    m_fft.debug_out();
    emit update_fft(&m_fft);
}

void MainWindow::if_error(int err)
{
  ui->statusBar->showMessage(QString("Inteface error %1").arg(err));
  if( m_iface->isOpen() )
  {
        m_iface->close();
  }
  ui->statusBar->showMessage("port closed");
}

void MainWindow::port_open()
{
  qDebug() << __PRETTY_FUNCTION__;
  if( !m_iface->isOpen() )
    {
      bool res = m_iface->open();
      ui->statusBar->showMessage(res?"Port opened":"Port not opened");
    }
}

void MainWindow::port_close()
{
  qDebug() << __PRETTY_FUNCTION__;
  if( m_iface->isOpen())
    {
      m_iface->close();
      ui->statusBar->showMessage("port closed");
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
  Q_UNUSED(event)
  ui_power1->close();
  ui_vectors->close();
}
