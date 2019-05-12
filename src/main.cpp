// The "Square Detector" program.
// It loads several images sequentially and tries to find squares in
// each image
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
/*#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "colordetection.hpp"
#include "opencv2/highgui/highgui.hpp"*/

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    Mat image = imread("images/rouge.jpg");
    namedWindow("image",CV_WINDOW_FREERATIO);
    imshow("image", image);

    Mat OutputImage;
    inRange(image,Scalar(0,0,125),Scalar(200,200,255), OutputImage); // Watch out Scalar in BGR

    namedWindow("Output",CV_WINDOW_FREERATIO);
    imshow("Output",OutputImage);
    waitKey(0);

}
