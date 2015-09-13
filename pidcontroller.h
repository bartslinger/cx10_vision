#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

#include <QObject>

#define FILTER_SIZE 5

class PIDController : public QObject
{
    Q_OBJECT
public:
    explicit PIDController(QObject *parent = 0);
    ~PIDController();
    void setGainP(double P);
    void setGainI(double I);
    void setGainD(double D);
    void resetIntegral();
    void setInterval(int msec);
    double propagate(double target, double measurement);

private:
    double gainP;
    double gainI;
    double gainD;
    int interval;
    double previous_errors[FILTER_SIZE];
    int prev_error_cnt;
    double integral_error;

signals:

public slots:
};

#endif // PIDCONTROLLER_H
