#ifndef RECORDER_H
#define RECORDER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QTimer>

enum States { IDLE, RECORDING, PLAYBACK };

class Recorder : public QObject
{
    Q_OBJECT
public:
    explicit Recorder(QObject *parent = 0);
    ~Recorder();

    void startRecording();
    void stopRecording();
    void startPlayback();
    void stopPlayback();

    void updateAxis(uint axis, uint value);


private:
    QTimer *timer;
    QFile *file;
    QTextStream filestream;

    uint axes[4];
    States state;

signals:
    void pwmPlayback(uint axis, uint value);

public slots:

private slots:
    void timerSlot();
};

#endif // RECORDER_H
