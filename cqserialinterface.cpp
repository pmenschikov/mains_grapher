#include "cqserialinterface.h"

CQSerialInterface::CQSerialInterface(QObject *parent) : QObject(parent)
{
    connect(&m_port,&QSerialPort::readyRead, this, &CQSerialInterface::new_data);
    connect(&m_port,&QSerialPort::errorOccurred, this, &CQSerialInterface::port_error);
    m_port.setFlowControl(QSerialPort::NoFlowControl);
}

void CQSerialInterface::new_data()
{
    QByteArray data = m_port.readAll();
    m_data_ready_cb(data.data(), data.length());
}

bool CQSerialInterface::send(char *data, int len)
{
    m_port.write(data, len);
    m_port.flush();
   return true;
}

bool CQSerialInterface::open()
{
    return m_port.open(QIODevice::ReadWrite);
}

void CQSerialInterface::close()
{
    m_port.close();
}

bool CQSerialInterface::isOpen()
{
    return m_port.isOpen();
}

void CQSerialInterface::port_error(QSerialPort::SerialPortError error)
{
    qDebug() << "Serial port error: " << error;

    if( error != QSerialPort::SerialPortError::NoError)
    {
        emit sig_error(static_cast<int>(error));
    }
}
