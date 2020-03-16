#ifndef CADCCTRL_INTERFACE_H
#define CADCCTRL_INTERFACE_H

#include <functional>

class CADCctrl_interface
{
public:
    typedef std::function<void (char*,int)> data_ready_callback_t;
public:
    virtual ~CADCctrl_interface() =default;
    virtual bool send(char*, int)=0;
    bool set_callback(data_ready_callback_t callback)
    {
           m_data_ready_cb = callback;
           return true;
    }

    virtual bool open() =0;
    virtual void close() =0;
    virtual bool isOpen() =0;


protected:
    data_ready_callback_t m_data_ready_cb;
};

#endif // CADCCTRL_INTERFACE_H
