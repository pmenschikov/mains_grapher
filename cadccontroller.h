#ifndef CADCCONTROLLER_H
#define CADCCONTROLLER_H

#include <functional>
#include "cadcctrl_interface.h"

class CADCcontroller
{
public:
    CADCcontroller();

    virtual ~CADCcontroller() = default;

    virtual void start() =0;
    virtual void stop() =0;
    virtual void new_data(char *, int)=0;

    virtual int raw_value(int channel) =0;
    virtual double value(int channel) =0;

    void set_interface(CADCctrl_interface *iface)
    {
        using namespace std::placeholders;
        m_interface = iface;
        m_interface->set_callback(std::bind(&CADCcontroller::new_data
                                            , this, _1,_2));
    }

protected:
    void test_cb(char *data, int len)
    {
        new_data(data,len);
    }
protected:
    CADCctrl_interface *m_interface;
};

#endif // CADCCONTROLLER_H
