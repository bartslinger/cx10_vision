#include "pidcontroller.h"
#include <QDebug>

PIDController::PIDController(QObject *parent) : QObject(parent),
    gainP(0),
    gainD(0),
    interval(40),
    prev_error_cnt(0),
    integral_error(0)
{
    for (int i=0;i<FILTER_SIZE;i++){
        previous_errors[i] = 0;
    }

}

PIDController::~PIDController()
{

}

void PIDController::setGainP(double P)
{
    gainP = P;
    qDebug() << "P: " << gainP;
}

void PIDController::setGainI(double I)
{
    gainI = I;
    qDebug() << "I: " << gainI;
}

void PIDController::setGainD(double D)
{
    gainD = D;
    qDebug() << "D: " << gainD;
}

void PIDController::resetIntegral()
{
    integral_error = 0;
}

void PIDController::setInterval(int msec)
{
    interval = msec;
}

double PIDController::propagate(double target, double measurement)
{
    double error = target - measurement;
    double difference = 0;
    for(int i=0; i<FILTER_SIZE;i++){
        difference += error - previous_errors[i];
    }

    // If line is lost, altitude 0 is given. Do not use for differential calculations
    if (measurement > 0){
        // Diff
        previous_errors[prev_error_cnt] = error;
        prev_error_cnt++;
        prev_error_cnt = prev_error_cnt % FILTER_SIZE;

        // Integral
        integral_error += error * interval / 1000;

    }
    qDebug() << gainI * integral_error;
    return gainP * (target - measurement) + (gainD * difference / interval) + (gainI * integral_error);
}

