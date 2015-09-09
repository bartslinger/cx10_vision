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
    void parameterUpdate(Scalar lower, Scalar upper);


private:
    QTimer *timer;
    VideoCapture cap;
    Mat currentFrame;

    Scalar lowerRange;
    Scalar upperRange;

    double getOrientation(vector<Point> &pts, Mat &img);
    void processFrame();


signals:
    void dataReady(double altitude, double direction, double offset);
public slots:

private slots:
    void processNextFrame();
};

#endif // VISIONMODULE_H
