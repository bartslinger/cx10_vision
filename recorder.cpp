#include "recorder.h"
#include <QDebug>

Recorder::Recorder(QObject *parent) : QObject(parent),
    state(IDLE)
{
    // Set timer for recording and playback
    timer = new QTimer(this);
    timer->setInterval(10);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    timer->start();

    // Set initial axes values to zero
    for(uint i=0;i<4;i++) axes[i] = 0;


    file = new QFile();
    filestream.setDevice(file);

}

Recorder::~Recorder()
{
    if (file->isOpen()) {
        file->close();
    }
    delete file;
    delete timer;
}

void Recorder::startRecording()
{
    file->setFileName("recording.txt");
    if(file->isOpen()) {
        file->close();
    }
    file->open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);

    state = RECORDING;
}

void Recorder::stopRecording()
{
    file->close();
    state = IDLE;
}

void Recorder::startPlayback(QString filename)
{
    if (!file->isOpen()) {
        file->setFileName(filename);
        file->open(QIODevice::ReadWrite | QIODevice::Text);
    }
    filestream.seek(0);
    state = PLAYBACK;
}

void Recorder::stopPlayback()
{
    file->close();
    state = IDLE;

}

void Recorder::setPitch(int pitch_correction)
{
    pitch_corr = pitch_correction;
}

void Recorder::updateAxis(uint axis, uint value)
{
    axes[axis] = value;
}

void Recorder::timerSlot()
{
    static int i = 0;
    switch (state) {
    case RECORDING:
        filestream << i++ << "," << axes[0] << "," << axes[1] << "," << axes[2] << "," << axes[3] << endl;
        break;

    case PLAYBACK:
        if(!filestream.atEnd()) {
            QString line = filestream.readLine();
            qDebug() << line;
            QStringList list = line.split(",");
            emit(pwmPlayback(0,list[1].toUInt()));
            emit(pwmPlayback(1,list[2].toUInt()));
            emit(pwmPlayback(2,list[3].toUInt()));
            if (list[1].toUInt() != 127) {
                emit(pwmPlayback(3,list[4].toUInt()-pitch_corr));
            } else {
                emit(pwmPlayback(3,list[4].toUInt()));
            }
        }
        else {
            qDebug() << "END";
            this->stopPlayback();
        }
        break;

    case IDLE:
    default:
        break;
    }
}

