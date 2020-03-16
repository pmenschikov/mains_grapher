#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>

#include "ccs5451_controller.h"
#include "cqserialinterface.h"

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

    virtual void timerEvent(QTimerEvent *);
private:
    void update_graphs();

    void createActions();

    QAction *actGain;
    QAction *actOwrs;

    QLabel *m_statusLeft;
    QLabel *m_statusMiddle;
    QLabel *m_statusRight;

    QString m_portname;
private:
    Ui::MainWindow *ui;

private:
    Plot *d_plot_voltages;
    Plot *d_plot_currents;
    int m_timer;

    CCS5451_controller *m_ctrl;
    CADCctrl_interface *m_iface;
};

#endif // MAINWINDOW_H
