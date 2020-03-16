#include <QDebug>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "ccs5451_controller.h"
#include "cadcctrl_interface.h"

CCS5451_controller::CCS5451_controller()
{

}

void CCS5451_controller::start()
{
    qDebug() << __PRETTY_FUNCTION__;
    m_interface->send(const_cast<char*>("e3\n"),3);
    sleep(1);
    m_interface->send(const_cast<char*>("a1\n"),3);
}

void CCS5451_controller::stop()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void CCS5451_controller::new_data(char *data,int len)
{
    put(QByteArray(data, len));
}

double  CCS5451_controller::value(int channel)
{
    Q_UNUSED(channel)
    return 0.0;
}

int CCS5451_controller::raw_value(int channel)
{
    Q_UNUSED(channel)
    return 0;
}

bool CCS5451_controller::open()
{
    return true;
  //  return m_interface->open();
}

void CCS5451_controller::set_owrs(bool on)
{
    send_cmd(on ? "c0":"c1");
}

void CCS5451_controller::set_gain(bool on)
{
    send_cmd( on ? "d0":"d1");
}

void CCS5451_controller::send_cmd(const char *cmd)
{
    if( m_interface->isOpen())
    {
        char cmd_buf[8];
        int len = snprintf(cmd_buf,8-1, "%s\n", cmd);
        len = m_interface->send(cmd_buf, len);
        //qDebug() << "send: " << len;
    }
}
