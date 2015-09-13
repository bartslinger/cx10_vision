#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGameController/QGameController>
#include <QTimer>
#include <QSettings>
#include "serialcontroller.h"
#include "recorder.h"
#include "visionmodule.h"
#include "altitudeplot.h"
#include "pidcontroller.h"
#include "binarycontroller.h"

enum Mode {KILL, ACTIVE};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void serialConnected(QString deviceName);
    void pwmPlayback(uint axis, uint value);
    void handleVisionData(double altitude, double direction, double offset);

private:
    Ui::MainWindow *ui;
    QSettings *settings;
    QGameController *gameController;
    Recorder *recorder;
    VisionModule *vision;
    PIDController *altitudeController;
    BinaryController *altBinaryController;
    PIDController *dirController;
    AltitudePlot *altitudePlot;
    QTimer *timer;
    char axis_moved;
    Mode mode;

    SerialController *serialController;

    void kill();
    void unkill();
    void sendHSVValues();

private slots:
    void handleAxisEvent(QGameControllerAxisEvent *event);
    void handleButtonEvent(QGameControllerButtonEvent *event);
    void on_onButton_clicked();
    void on_offButton_clicked();
    void on_pushButton_2_clicked();
    void on_configureButton_clicked();
    void on_recordStartButton_clicked();
    void on_recordStopButton_clicked();
    void on_playbackStartButton_clicked();
    void on_Hmin_valueChanged(int arg1);
    void on_Hmax_valueChanged(int arg1);
    void on_Smin_valueChanged(int arg1);
    void on_Smax_valueChanged(int arg1);
    void on_Vmin_valueChanged(int arg1);
    void on_Vmax_valueChanged(int arg1);
    void on_visionPause_clicked();
    void on_visionContinue_clicked();
    void on_altP_valueChanged(double arg1);
    void on_altD_valueChanged(double arg1);
    void on_altControl_stateChanged(int arg1);
    void on_altTarget_valueChanged(double arg1);
    void on_altNeutral_valueChanged(int arg1);
    void on_altI_valueChanged(double arg1);
    void on_altBinUp_valueChanged(int arg1);
    void on_altBinDown_valueChanged(int arg1);
    void on_altBinControl_stateChanged(int arg1);
    void on_dirP_valueChanged(double arg1);
    void on_playbackCorrection_valueChanged(int arg1);
};

#endif // MAINWINDOW_H
