#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGameController/QGameController>
#include <QTimer>
#include "serialcontroller.h"
#include "recorder.h"

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

private:
    Ui::MainWindow *ui;
    QGameController *gameController;
    Recorder *recorder;
    QTimer *timer;
    char axis_moved;
    Mode mode;

    SerialController *serialController;

    void kill();
    void unkill();

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
};

#endif // MAINWINDOW_H
