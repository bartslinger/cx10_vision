#ifndef VISIONMODULE_H
#define VISIONMODULE_H

#include <QObject>
#include <QTimer>
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;


class VisionModule : public QObject
{
    Q_OBJECT
public:
    explicit VisionModule(QObject *parent = 0);
    ~VisionModule();

    void start();
    void pause();
    void parameterUpdate(int Hmin, int Smin, int Vmin, int Hmax, int Smax, int Vmax);


private:
    QTimer *timer;
    VideoCapture cap;
    Mat currentFrame;
    VideoWriter video_writer;

    Scalar lowerRange;
    Scalar upperRange;

    double getOrientation(vector<Point> &pts);
    void processFrame();


signals:
    void dataReady(double altitude, double direction, double offset);
public slots:

private slots:
    void processNextFrame();
};

#endif // VISIONMODULE_H
