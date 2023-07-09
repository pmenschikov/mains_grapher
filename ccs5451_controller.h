#ifndef CCS5451_CONTROLLER_H
#define CCS5451_CONTROLLER_H

#include <QObject>
#include "cadccontroller.h"
#include "cdatabufffer.h"
#include "cmeasurements.h"
#include "ccalibrationdata.h"


class CCS5451_controller:
    public QObject,
    public CADCcontroller,
    public CDataBuffer
{
  Q_OBJECT
public:
    CCS5451_controller();
    ~CCS5451_controller();

    void start();
    void stop();
    void new_data(char*,int);

    bool open();

    void set_owrs(bool);
    void set_gain(bool);

    int raw_value(int channel);
    double value(int channel);

    adc_data extract_data(QByteArray&);
    void translate(adc_data&);

    QVector<int16_t> zero();

    CMeasurements *measurements();

    float rms_voltage(int channel);
    float rms_current(int channel);
    float p_power(int channel);
    float s_power(int channel);
    float pf(int channel);

    CCalibrationData& get_calibration();
    void set_calibration(CCalibrationData);

signals:
    bool sig_error(int);
public slots:
    void error_slot(int);

protected:
    void send_cmd(const char*);

protected:
    bool m_gain;
    CMeasurements* m_measurements;

    QVector<int16_t> m_offsets;

    CCalibrationData m_calibration;
    unsigned int m_current_data_num;
    unsigned int m_prev_fail_data_num;
};

#endif // CCS5451_CONTROLLER_H
