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
    VideoCapture cap(0); //capture the video from web cam

    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
         return -1;
    }

    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

     int iLowH = 0;
     int iHighH = 179;

     int iLowS = 0;
     int iHighS = 255;

     int iLowV = 0;
     int iHighV = 255;

    char* nameButtonRgb = "Rgb mode";
    int callbackButton = 1;

     //Create trackbars in "Control" window
    cvCreateTrackbar("Low Hue/Blue", "Control", &iLowH, 179); //Hue (0 - 179)
    cvCreateTrackbar("High Hue/Blue", "Control", &iHighH, 179);

    cvCreateTrackbar("Low Saturation/Green", "Control", &iLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("High Saturation/Green", "Control", &iHighS, 255);

    cvCreateTrackbar("Low Brightness/Red", "Control", &iLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("High Brightness/Red", "Control", &iHighV, 255);

    //cvCreateButton(nameButtonRgb, callbackButton, nameButtonRgb, CV_RADIOBOX, 1);
    //cvCreateButton(const char *button_name = NULL, optional CvButtonCallback on_change = NULL, optional void *userdata = NULL, optional int button_type = CV_PUSH_BUTTON, optional int initial_button_state = 0)

    while (true)
    {
        Mat imgOriginal;

        bool bSuccess = cap.read(imgOriginal); // read a new frame from video

         if (!bSuccess) //if not success, break loop
        {
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }

        Mat imgHSV;

        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

        Mat imgThresholded;

        //inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
        inRange(imgOriginal, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
        //inRange(imgOriginal, Scalar(0,0,125),Scalar(200,200,255), imgThresholded); //Threshold the image

        //morphological opening (remove small objects from the foreground)

        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        //morphological closing (fill small holes in the foreground)
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        imshow("Thresholded Image", imgThresholded); //show the thresholded image

        imshow("Original", imgOriginal); //show the original image

        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
            break;
       }
    }







    Mat image = imread("images/img3.jpg");
    namedWindow("image",CV_WINDOW_FREERATIO);
    imshow("image", image);

    Mat OutputImage;
    inRange(image,Scalar(0,0,125),Scalar(100,100,255), OutputImage); // Watch out Scalar in BGR

    namedWindow("Output",CV_WINDOW_FREERATIO);
    imshow("Output",OutputImage);
    waitKey(0);

    return 0;

}
