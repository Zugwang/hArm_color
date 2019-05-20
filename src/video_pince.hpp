#ifndef VIDEO_PINCE
#define VIDEO_PINCE

#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

void hello_world();
void thresh_callback(int, void* , cv::Mat src, int thresh, cv::Mat output);

class Video_pince
{
    public:
};


#endif
