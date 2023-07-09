#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>

#include "ccs5451_controller.h"
#include "cqserialinterface.h"
#include "powers_widget.h"
#include "cfft.h"
#include "vectordiagramwidget.h"


namespace Ui {
class MainWindow;
}

class Plot;
class QLabel;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void actionStart();
    void actionStop();

    void actionCmdStart();
    void actionGain(bool);
    void actionOwrs(bool);

    void if_error(int);

    void port_open();
    void port_close();

    void ui_power();
    void ui_vectors_show();

    void adc_calibrate();
    void calibration_dialog();

    virtual void timerEvent(QTimerEvent *);
    void closeEvent(QCloseEvent *event);

signals:
    void update_powers(CCS5451_controller*);
    void update_fft(CFFT*);
private:
    void update_graphs();

    void createActions();

	void create_fft_plans();

    QAction *actGain;
    QAction *actOwrs;

    QLabel *m_statusLeft;
    QLabel *m_statusMiddle;
    QLabel *m_statusRight;

    QString m_portname;
private:
    powers_widget *ui_power1;
    VectorDiagramWidget *ui_vectors;
    Ui::MainWindow *ui;

private:
    Plot *d_plot_voltages;
    Plot *d_plot_currents;
    int m_timer;

    CCS5451_controller *m_ctrl;
    CADCctrl_interface *m_iface;

    CFFT m_fft;
};

#endif // MAINWINDOW_H
