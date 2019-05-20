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

    while (true)
    {
        test = control_panel(test);

        Mat imgOriginal; //drawing;

        bool bSuccess = cap.read(imgOriginal); // read a new frame from video

         if (!bSuccess) //if not success, break loop
        {
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }

        color_detection(imgOriginal, test, );


        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
            break;
       }
    }

    waitKey(0);
    return 0;

}
