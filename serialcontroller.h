#ifndef SERIALCONTROLLER_H
#define SERIALCONTROLLER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>

#define LOW 0
#define HIGH 255

#define FLIP_PIN 4
#define MODE_PIN 5
#define ENABLE_PIN 6

#define THROTTLE_PIN 10
#define YAW_PIN 11
#define PITCH_PIN 3
#define ROLL_PIN 9

class SerialController : public QObject
{
    Q_OBJECT
public:
    explicit SerialController(QObject *parent = 0);
    ~SerialController();
    void start();
    void enable();
    void disable();
    void pwmMode();
    void setThrust(int thrustValue);
    void setRudder(int value);
    void setPitch(int value);
    void setRoll(int value);
    void flipPush();
    void flipRelease();
    void incrAeroMode();
    void debugReadAll();

private:
    QString deviceName;
    QSerialPort *serial;
    void findDevice();
    int port_val[3];
    QTimer *timer;
    QTimer *openTimer;

    void analogWrite(int pin, int value);
    void digitalWrite(uint pin, uint value);
    void setModePWM(uint pin);

signals:
    void controllerConnected(QString deviceName);

public slots:

private slots:
    void releaseAeroButton();
    void tryOpenDevice();
};

#endif // SERIALCONTROLLER_H
