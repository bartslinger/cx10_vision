#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mode(ACTIVE)
{
    ui->setupUi(this);
    gameController = new QGameController(0, this);
    if (gameController->isValid()) {
        connect(gameController, SIGNAL(gameControllerAxisEvent(QGameControllerAxisEvent*)), this, SLOT(handleAxisEvent(QGameControllerAxisEvent*)));
        connect(gameController, SIGNAL(gameControllerButtonEvent(QGameControllerButtonEvent*)), this, SLOT(handleButtonEvent(QGameControllerButtonEvent*)));
    }
    timer = new QTimer(this);
    timer->setInterval(15);
    connect(timer, SIGNAL(timeout()), gameController, SLOT(readGameController()));
    timer->start();

    // Set up the serial controller (Arduino)
    serialController = new SerialController();
    connect(serialController, SIGNAL(controllerConnected(QString)), this, SLOT(serialConnected(QString)));
    serialController->start();

    axis_moved = 0x00;
    recorder = new Recorder();
    connect(recorder, SIGNAL(pwmPlayback(uint,uint)), this, SLOT(pwmPlayback(uint,uint)));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete gameController;
    delete timer;
    delete recorder;
}

void MainWindow::serialConnected(QString deviceName)
{
    qDebug() << deviceName;
    ui->configureButton->setEnabled(true);
}

void MainWindow::pwmPlayback(uint axis, uint value)
{
    switch(axis) {
    case 0:
        serialController->setThrust(value);
        break;
    case 1:
        serialController->setRudder(value);
        break;
    case 2:
        serialController->setRoll(value);
        break;
    case 3:
        serialController->setPitch(value);
        break;
    }
}

void MainWindow::kill()
{
    this->mode = KILL;
    recorder->stopPlayback();
    serialController->setThrust(0);
}

void MainWindow::unkill()
{
    this->mode = ACTIVE;
}

void MainWindow::handleAxisEvent(QGameControllerAxisEvent *event)
{

    float value = (1-event->value())*127.5;
    int pwm = value  / 1;
    float inverseValue = (1+event->value())*127.5;
    int inversepwm = inverseValue / 1;


    // SWITCH TOP LEFT
    if(event->axis() == 3){
        if(event->value() > 0) {
            qDebug() << "Kill";
            this->kill();
            //serialController->flipPush();
        } else {
            qDebug() << "Unkill";
            this->unkill();
            //serialController->flipRelease();
        }
    }

    // Do not process commands if not active
    if(this->mode != ACTIVE) {
        delete event;
        return;
    }

    // THRUST
    if(event->axis() == 2){
        ui->throttleSlider->setValue(-event->value()*1000);
        serialController->setThrust(pwm);
        recorder->updateAxis(0, pwm);
        axis_moved |= (1<<0);
    }

    // RUDDER
    if(event->axis() == 5){
        ui->rudderSlider->setValue(event->value()*1000);
        serialController->setRudder(inversepwm);
        recorder->updateAxis(1, inversepwm);
        axis_moved |= (1<<1);
    }

    // ROLL
    if(event->axis() == 0){
        ui->rollSlider->setValue(event->value()*1000);
        serialController->setRoll(pwm);
        recorder->updateAxis(2, pwm);
        axis_moved |= (1<<2);
    }

    // PITCH
    if(event->axis() == 1){
        ui->pitchSlider->setValue(-event->value()*1000);
        serialController->setPitch(inversepwm);
        recorder->updateAxis(3, inversepwm);
        axis_moved |= (1<<3);
    }

    delete event;

    if (axis_moved == 0x0F){
        ui->onButton->setEnabled(true);
    }
}

void MainWindow::handleButtonEvent(QGameControllerButtonEvent *event)
{
    delete event;
}

void MainWindow::on_onButton_clicked()
{
    serialController->enable();
}

void MainWindow::on_offButton_clicked()
{
    serialController->disable();
}

void MainWindow::on_pushButton_2_clicked()
{
    serialController->incrAeroMode();
}

void MainWindow::on_configureButton_clicked()
{
    serialController->pwmMode();
}

void MainWindow::on_recordStartButton_clicked()
{
    recorder->startRecording();
}

void MainWindow::on_recordStopButton_clicked()
{
    recorder->stopRecording();
}

void MainWindow::on_playbackStartButton_clicked()
{
    recorder->startPlayback();
}
