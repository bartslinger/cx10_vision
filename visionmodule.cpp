#include "visionmodule.h"
#include <QDebug>
#include <QDateTime>

// 1 for playback, 0 for live recording and save to file
#define VISION_PLAYBACK 0

VisionModule::VisionModule(QObject *parent) : QObject(parent)
{
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(processNextFrame()));
    timer->setInterval(40);
#if VISION_PLAYBACK
    cap = VideoCapture("/home/bart/Documents/IMAV2015/arena_drone.mp4");
    timer->start();
#else
    cap = VideoCapture(1);
    cap >> currentFrame;
    const QDateTime now = QDateTime::currentDateTime();
    const QString timestamp = now.toString(QLatin1String("yyyyMMdd-hhmmsszzz"));
    const QString filename = QString::fromLatin1("/home/bart/Documents/IMAV2015/recordings/rec-%1.mpg").arg(timestamp);
    video_writer = VideoWriter(filename.toStdString(),CV_FOURCC('H','2','6','4'), 25.0, currentFrame.size());
#endif
    cap.set(CV_CAP_PROP_BUFFERSIZE, 0);
    namedWindow("lol", 1);
    moveWindow("lol", 1200, 200);
    lowerRange = Scalar(14,0,0);
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

void VisionModule::parameterUpdate(int Hmin, int Smin, int Vmin, int Hmax, int Smax, int Vmax)
{
    lowerRange.val[0] = Hmin;
    lowerRange.val[1] = Smin;
    lowerRange.val[2] = Vmin;
    upperRange.val[0] = Hmax;
    upperRange.val[1] = Smax;
    upperRange.val[2] = Vmax;

    processFrame();
}

void VisionModule::processNextFrame()
{
    cap >> currentFrame;

    // If no data, stop here
    if (currentFrame.empty()) return;

    // Camera is upside-down, rotate it 180deg
    flip(currentFrame, currentFrame, -1);

#if !VISION_PLAYBACK
    video_writer << currentFrame;
#endif
    // Cut outer boundaries (they are noisy
    Rect roi = Rect(20, 20, currentFrame.cols-40, currentFrame.rows-40);
    currentFrame = currentFrame(roi);

    processFrame();
}

double VisionModule::getOrientation(vector<Point> &pts)
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

    Mat displayFrame;
    currentFrame.copyTo(displayFrame);

    // Filter line/rope with color range (result is a binary image)
    Mat hsv_image;
    Mat range(currentFrame.rows, currentFrame.cols, CV_8U);
    cvtColor(currentFrame, hsv_image, COLOR_BGR2HSV);
    inRange(hsv_image, lowerRange, upperRange, range);

    // Erode / Delute (noise filter)
    int dilation_size = 5;
    Mat element = getStructuringElement( MORPH_RECT,
                                          Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                          Point( dilation_size, dilation_size ) );
    dilate(range, range, element);

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
    if (largestArea < 10000) {
        emit dataReady(0, 0, 0);
        imshow("lol", currentFrame);
        return;

    }


    // Retreive angle from the largest contour using PCA, in degrees. Forward = 0deg
    double angle = getOrientation(contours[largestIdx]) * 180 / M_PI - 90;

    // Determine length of the rope from minimum rectangle around contour
    RotatedRect minRect = minAreaRect( Mat(contours[largestIdx]));
    Point2f rect_points[4]; minRect.points( rect_points );
    double length1 = norm(rect_points[0] - rect_points[1]);
    double length2 = norm(rect_points[1] - rect_points[2]);
    double length = length1;
    if(length2>length1) length = length2;

    // Calculate height (no units)
    double width = largestArea/length;
    double height = 1000/width;

    // Draw the largest contour on the original frame and show it
    Scalar color = Scalar(128, 128, 128);
    drawContours( displayFrame, contours, largestIdx, color, 2, 8, hierarchy, 0, Point() );

    // Emit calculated parameters
    emit dataReady(height, angle, 0);
    imshow("lol", displayFrame);
}
