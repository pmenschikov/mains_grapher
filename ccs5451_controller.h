#ifndef CCS5451_CONTROLLER_H
#define CCS5451_CONTROLLER_H

#include "cadccontroller.h"
#include "cdatabufffer.h"


class CCS5451_controller:
        public CADCcontroller,
        public CDataBuffer
{
public:
    CCS5451_controller();

    void start();
    void stop();
    void new_data(char*,int);

    bool open();

    void set_owrs(bool);
    void set_gain(bool);

    int raw_value(int channel);
    double value(int channel);

protected:
    void send_cmd(const char*);

};

#endif // CCS5451_CONTROLLER_H
