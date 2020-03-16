#ifndef CQSERIALINTERFACE_H
#define CQSERIALINTERFACE_H

#include <QObject>
#include <QtSerialPort>

#include "cadcctrl_interface.h"

class CQSerialInterface : public QObject,
        public CADCctrl_interface
{
    Q_OBJECT
public:
    explicit CQSerialInterface(QObject *parent = nullptr);

    void set_port_name(QString name)
    {
        m_port.setPortName(name);
    }

    bool send(char *, int);
    bool open();
    void close();
    bool isOpen();

signals:

public slots:
   void new_data();
private:
    QSerialPort m_port;
};

#endif // CQSERIALINTERFACE_H
