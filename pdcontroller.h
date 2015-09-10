#ifndef PDCONTROLLER_H
#define PDCONTROLLER_H

#include <QObject>

#define FILTER_SIZE 5

class PDController : public QObject
{
    Q_OBJECT
public:
    explicit PDController(QObject *parent = 0);
    ~PDController();
    void setGainP(double P);
    void setGainD(double D);
    void setInterval(int msec);
    double propagate(double target, double measurement);

private:
    double gainP;
    double gainD;
    int interval;
    double previous_errors[FILTER_SIZE];
    int prev_error_cnt;

signals:

public slots:
};

#endif // PDCONTROLLER_H
