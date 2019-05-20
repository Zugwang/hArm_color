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

    //create a window called "Control"

    myThreshold test;

    test = control_panel(test);





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
