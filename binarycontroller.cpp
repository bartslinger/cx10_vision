#include "binarycontroller.h"
#include <QDebug>

BinaryController::BinaryController(QObject *parent) : QObject(parent),
    up_value(0),
    down_value(0)
{

}

BinaryController::~BinaryController()
{

}

void BinaryController::setUpValue(int up)
{
    up_value = up;
    qDebug() << "Up: " << up_value;
}

void BinaryController::setDownValue(int down)
{
    down_value = down;
    qDebug() << "Down: " << down_value;
}

double BinaryController::propagate(double target, double measurement)
{
    if (measurement > target) {
        return down_value;
    } else {
        return up_value;
    }
}

