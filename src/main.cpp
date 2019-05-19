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


void thresh_callback(int, void* , Mat src, int thresh);


int main(int argc, char** argv)
{
    VideoCapture cap("images/video_test2.mp4"); //capture the video from web cam

    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
         return -1;
    }

    Mat image = imread("images/cubes.jpg");

    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

    int buttonHSV = 1;

    int iLowH = 75;
    int iHighH = 158;

    int iLowS = 120;
    int iHighS = 232;

    int iLowV = 117;
    int iHighV = 255;

    int thresh = 100;
    int max_thresh = 255;



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
        Mat imgOriginal;
        Mat imgHSV;
        Mat imgThresholded;

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
        //inRange(imgOriginal, Scalar(0,0,125),Scalar(200,200,255), imgThresholded); //Threshold the image

        //morphological opening (remove small objects from the foreground)

        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        //morphological closing (fill small holes in the foreground)
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        //cvtColor(imgThresholded, imgThresholdedGrayscale, CV_BGR2GRAY);
        Mat imgThresholdedCircle = imgThresholded;
        //GaussianBlur(imgThresholdedCircle, imgThresholdedCircle, Size(9,9), 2, 2);

        //cout << "VECTOOOOOOOOOOOOOOOOOOOR" << endl;
        vector<Vec3f> circles;
        HoughCircles(imgThresholdedCircle, circles, CV_HOUGH_GRADIENT, 1, imgThresholdedCircle.rows/8, 400, 50, 0, 50);

        //HoughCircles(InputArray image, OutputArray circles, int method, double dp, double minDist, optional double param1 = 100, optional double param2 = 100, optional int minRadius = 0, optional int maxRadius = 0)
        for(size_t i = 0; i < circles.size(); i++)
        {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);

            circle(imgOriginal, center, 3, Scalar(0,255,0), -1, 8, 0);
            circle(imgOriginal, center, radius, Scalar(0,0,255), 3, 8, 0);
        }

        thresh_callback( 0, 0 , imgThresholded, thresh);

        namedWindow("Original", CV_WINDOW_FREERATIO);
        namedWindow("Thresholded Image", CV_WINDOW_FREERATIO);
        //namedWindow("Thresholded Image Circle", CV_WINDOW_FREERATIO);
        //imshow("Thresholded Image Circle", imgThresholdedCircle); //show the thresholded image
        imshow("Thresholded Image", imgThresholded); //show the thresholded image
        imshow("Original", imgOriginal); //show the original image



        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
            break;
       }
    }

    waitKey(0);


    return 0;

}

void thresh_callback(int, void*, Mat src,int thresh)
{
  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using canny
  Canny( src, canny_output, thresh, thresh*2, 3 );
  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Draw contours
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
    for(unsigned int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( 255,0,0 );
        drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
    }
    Rect rectangle;
    vector<Point> tab;

    for (unsigned int i = 0 ; i < contours.size(); i++)
    {

        rectangle = boundingRect (contours[i]);
        Point c;
        c.x = rectangle.x + rectangle.width/2; c.y = rectangle.y + rectangle.height/2;
        circle(drawing, c, 50, (0,255,0));
        tab.push_back(c);

    }


  /// Show in a window
  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  imshow( "Contours", drawing );
}
