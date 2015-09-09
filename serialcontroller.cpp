#include "serialcontroller.h"
#include <QtDebug>

SerialController::SerialController(QObject *parent) : QObject(parent)
{
    timer  = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(releaseAeroButton()));
    timer->setSingleShot(true);

    openTimer = new QTimer(this);
    connect(openTimer, SIGNAL(timeout()), this, SLOT(tryOpenDevice()));

    serial = new QSerialPort;
    serial->setBaudRate(57600);
    port_val[0] = 0x00;
    port_val[1] = 0x00;
    port_val[2] = 0x00;
}

SerialController::~SerialController()
{
    if(serial->isOpen()) {
        serial->close();
    }
    delete serial;
    delete timer;
    delete openTimer;
}

void SerialController::start()
{
    findDevice();
}

void SerialController::enable()
{
    digitalWrite(ENABLE_PIN, HIGH);
}

void SerialController::disable()
{
    digitalWrite(ENABLE_PIN, LOW);
}

void SerialController::pwmMode()
{
    setModePWM(THROTTLE_PIN);
    setModePWM(YAW_PIN);
    setModePWM(PITCH_PIN);
    setModePWM(ROLL_PIN);
}

void SerialController::setThrust(int thrustValue)
{
    analogWrite(10, thrustValue);
}

void SerialController::setRudder(int value)
{
    analogWrite(11, value);
}

void SerialController::setPitch(int value)
{
    analogWrite(3, value);
}

void SerialController::setRoll(int value)
{
    analogWrite(9, value);
}

void SerialController::flipPush()
{
    digitalWrite(FLIP_PIN, HIGH);
}

void SerialController::flipRelease()
{
    digitalWrite(FLIP_PIN, LOW);
}

void SerialController::incrAeroMode()
{
    digitalWrite(MODE_PIN, HIGH);
    timer->start(200);
}

void SerialController::debugReadAll()
{
    qDebug() << serial->readAll();
}

void SerialController::findDevice()
{
    QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();
    for(int i=0; i<list.size(); i++){
        if(QString::compare(list[i].description(), "Arduino Uno") == 0){
            serial->setPort(list[i]);
            deviceName = list[i].systemLocation();
            serial->setBaudRate(57600);
            openTimer->start(500);
        }
    }

}

void SerialController::analogWrite(int pin, int value)
{
    if (!serial->isOpen())
        return;
    char buff[3];
    buff[0] = 0xE0 | pin;
    buff[1] = value & 0x7F;
    buff[2] = (value >> 7) & 0x7F;
    serial->write(buff, 3);
    serial->waitForBytesWritten(1000);
    while(serial->waitForReadyRead(1)) {
        serial->readAll();
    }
}

void SerialController::digitalWrite(uint pin, uint value)
{
    int port_num = pin / 8;

    switch(value) {
    case LOW:
        port_val[port_num] &= ~(1<<pin);
        break;
    case HIGH:
        port_val[port_num] |= (1<<pin);
        break;
    default:
        return;
    }

    char buff[3];
    buff[0] = 0x90 | port_num;
    buff[1] = port_val[port_num] & 0x7F;
    buff[2] = (port_val[port_num] >> 7) & 0x7F;
    serial->write(buff, 3);
    serial->waitForBytesWritten(1000);
    while(serial->waitForReadyRead(10)) {
        serial->readAll();
    }
}

void SerialController::setModePWM(uint pin)
{
    char buff[3];
    buff[0] = 0xF4;
    buff[1] = pin;
    buff[2] = 0x03;
    serial->write(buff, 3);
    serial->waitForBytesWritten(1000);
    while(serial->waitForReadyRead(10)) {
        serial->readAll();
    }
}

void SerialController::releaseAeroButton()
{
    qDebug() << "release";
    digitalWrite(MODE_PIN, LOW);
}

void SerialController::tryOpenDevice()
{
    if(serial->open(QIODevice::ReadWrite)) {
        qDebug() << "OPEN";
        openTimer->stop();
        emit controllerConnected(deviceName);
    } else {
        qDebug() << "DICHT";
    }
}
