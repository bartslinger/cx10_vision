#include "pdcontroller.h"
#include <QDebug>

PDController::PDController(QObject *parent) : QObject(parent),
    gainP(0),
    gainD(0),
    interval(40),
    prev_error_cnt(0)
{
    for (int i=0;i<FILTER_SIZE;i++){
        previous_errors[i] = 0;
    }

}

PDController::~PDController()
{

}

void PDController::setGainP(double P)
{
    gainP = P;
}

void PDController::setGainD(double D)
{
    gainD = D;
}

void PDController::setInterval(int msec)
{
    interval = msec;
}

double PDController::propagate(double target, double measurement)
{
    double error = target - measurement;
    double difference = 0;
    for(int i=0; i<FILTER_SIZE;i++){
        difference += error - previous_errors[i];
    }

    // If line is lost, altitude 0 is given. Do not use for differential calculations
    if (measurement > 0){
        previous_errors[prev_error_cnt] = error;
        prev_error_cnt++;
        prev_error_cnt = prev_error_cnt % FILTER_SIZE;
    }
    qDebug() << (int)(gainD * difference / interval);

    //return (gainD * difference * 1000 / interval);
    return gainP * (target - measurement) + (gainD * difference / interval);
}

