#include "visionmodule.h"
#include <QDebug>

VisionModule::VisionModule(QObject *parent) : QObject(parent)
{
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(processNextFrame()));
    timer->setInterval(40);
    cap = VideoCapture("/home/bart/Documents/IMAV2015/hover_2.mp4");
    //cap = VideoCapture(0);
    cap.set(CV_CAP_PROP_BUFFERSIZE, 0);
    namedWindow("lol", 1);
    moveWindow("lol", 800, 200);
    timer->start();
}

VisionModule::~VisionModule()
{
    delete timer;
}

void VisionModule::start()
{
    timer->start();
}

void VisionModule::pause()
{
    timer->stop();
}

void VisionModule::parameterUpdate(Scalar lower, Scalar upper)
{
    lowerRange = lower;
    upperRange = upper;
    processFrame();
}

void VisionModule::processNextFrame()
{
    cap >> currentFrame;
    processFrame();
}

double VisionModule::getOrientation(vector<Point> &pts, Mat &img)
{
    //Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    //Construct a buffer used by the pca analysis
    Mat data_pts = Mat(pts.size(), 2, CV_64FC1);
    for (int i = 0; i < data_pts.rows; ++i)
    {
        data_pts.at<double>(i, 0) = pts[i].x;
        data_pts.at<double>(i, 1) = pts[i].y;
    }

    //Perform PCA analysis
    PCA pca_analysis(data_pts, Mat(), CV_PCA_DATA_AS_ROW);

    //Store the position of the object
    Point pos = Point(pca_analysis.mean.at<double>(0, 0),
                      pca_analysis.mean.at<double>(0, 1));

    //Store the eigenvalues and eigenvectors
    vector<Point2d> eigen_vecs(2);
    vector<double> eigen_val(2);
    for (int i = 0; i < 2; ++i)
    {
        eigen_vecs[i] = Point2d(pca_analysis.eigenvectors.at<double>(i, 0),
                                pca_analysis.eigenvectors.at<double>(i, 1));

        eigen_val[i] = pca_analysis.eigenvalues.at<double>(0, i);
    }

    // Draw the principal components
    //circle(img, pos, 3, color, 2);
    //line(img, pos, pos + 0.02 * Point(eigen_vecs[0].x * eigen_val[0], eigen_vecs[0].y * eigen_val[0]) , color);
    //line(img, pos, pos + 0.02 * Point(eigen_vecs[1].x * eigen_val[1], eigen_vecs[1].y * eigen_val[1]) , color);

    return atan2(eigen_vecs[0].y, eigen_vecs[0].x);
}

void VisionModule::processFrame()
{

    // Return if there is no new frame (movie for example)
    if (currentFrame.empty()) return;

    // Cut outer boundaries (they are noisy
    Rect roi = Rect(20, 20, currentFrame.cols-40, currentFrame.rows-40);
    currentFrame = currentFrame(roi);

    // Filter line/rope with color range (result is a binary image)
    Mat hsv_image;
    Mat range(currentFrame.rows, currentFrame.cols, CV_8U);
    cvtColor(currentFrame, hsv_image, COLOR_BGR2HSV);
    inRange(hsv_image, lowerRange, upperRange, range);

    // Find the contours
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Mat rangeCopy;
    range.copyTo(rangeCopy);
    findContours( rangeCopy, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    // Find the largest area of all contours
    int largestIdx = 0;
    double largestArea = 0;
    for (unsigned int i = 0; i < contours.size(); i++) {
        double area = contourArea(contours[i]);
        if (area > largestArea){
            largestArea = area;
            largestIdx = i;
        }
    }

    // Continue only if there is at least one region identified
    if (largestArea > 0) {
        double angle = getOrientation(contours[largestIdx], range);
        qDebug() << angle;
    }

    imshow("lol", currentFrame);
}
