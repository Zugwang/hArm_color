#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include "video_pince.hpp"

using namespace cv;
using namespace std;



int main(int argc, char** argv)
{
    hello_world();
    VideoCapture cap("images/video_test2.mp4"); //capture the video from web cam

    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
         return -1;
    }

    Mat image = imread("images/cubes.jpg");

    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

    int buttonHSV = 1;

    int iLowH = 75;    int iHighH = 158;

    int iLowS = 120;    int iHighS = 232;

    int iLowV = 117;    int iHighV = 255;

    int thresh = 100;    int max_thresh = 255;



     //Create trackbars in "Control" window
    cvCreateTrackbar( " Canny thresh:", "Control", &thresh, max_thresh);
    cvCreateTrackbar("  RGB / HSB    ", "Control", &buttonHSV, 1);
    cvCreateTrackbar("Low Hue/Blue   ", "Control", &iLowH, 255); //Hue (0 - 179)
    cvCreateTrackbar("High Hue/Blue  ", "Control", &iHighH, 255);
    cvCreateTrackbar("Low Sat/Green  ", "Control", &iLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("High Sat/Green ", "Control", &iHighS, 255);
    cvCreateTrackbar("Low Bright/Red ", "Control", &iLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("High Bright/Red", "Control", &iHighV, 255);

    //cvCreateButton(const char *button_name = NULL, optional CvButtonCallbackon_change = NULL, optional void *userdata = NULL, optional int button_type = CV_PUSH_BUTTON, optional int initial_button_state = 0)

    //imshow("image",image);

    while (true)
    {
        Mat imgOriginal, imgHSV, imgThresholded,drawing;

        bool bSuccess = cap.read(imgOriginal); // read a new frame from video

         if (!bSuccess) //if not success, break loop
        {
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }



        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV


        if(buttonHSV == 0){
            inRange(imgOriginal, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
            //cout << "RGB = " << buttonHSV <<endl;
        }else{
            inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
            //cout << "HSV =" << buttonHSV <<endl;
        }

        //morphological opening (remove small objects from the foreground)
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        //morphological closing (fill small holes in the foreground)
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        Mat imgThresholdedCircle = imgThresholded;
        //GaussianBlur(imgThresholdedCircle, imgThresholdedCircle, Size(9,9), 2, 2);

        vector<Vec3f> circles;
        HoughCircles(imgThresholdedCircle, circles, CV_HOUGH_GRADIENT, 1, imgThresholdedCircle.rows/8, 400, 50, 0, 50);

        for(size_t i = 0; i < circles.size(); i++)
        {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);

            circle(imgOriginal, center, 3, Scalar(0,255,0), -1, 8, 0);
            circle(imgOriginal, center, radius, Scalar(0,0,255), 3, 8, 0);
        }

        thresh_callback( 0, 0 , imgThresholded, thresh, drawing);

        namedWindow("Original", CV_WINDOW_FREERATIO);
        namedWindow("Thresholded Image", CV_WINDOW_FREERATIO);
        namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
        imshow("Thresholded Image", imgThresholded);
        imshow("Original", imgOriginal);




        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
            break;
       }
    }

    waitKey(0);
    return 0;

}
