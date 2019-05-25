#ifndef VIDEO_PINCE
#define VIDEO_PINCE

#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

struct myThreshold
{
    int buttonHSV = 1;
    int iLowH = 75;    int iHighH = 158;
    int iLowS = 120;    int iHighS = 232;
    int iLowV = 117;    int iHighV = 255;
    int thresh = 100;    //int max_thresh = 255;
};

void hello_world();
std::vector<cv::Point2f> thresh_callback(int, void* , cv::Mat src, int thresh, cv::Mat output);

myThreshold control_panel(myThreshold src_);
std::vector<cv::Point2f> color_detection(cv::Mat imgOriginal, myThreshold test);

class Video_pince
{
    public:
};


#endif
