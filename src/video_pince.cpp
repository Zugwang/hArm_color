#include "video_pince.hpp"

using namespace std;
using namespace cv;


void hello_world()
{
    cout << "Hello world" << endl;
}

vector<Point> maFonction(int a)
{
    for(int i = 0; i < a ; i++)
    {
        vector<Point> batman (a);

    }
}

void myFuncPanel(int value, void *userdata)
{
    myThreshold pushed_threshold = *((myThreshold*)&userdata);
}

myThreshold control_panel(myThreshold src_)
{
    namedWindow("Control", CV_WINDOW_AUTOSIZE);

    int buttonHSV = 1;
    int iLowH = 75;    int iHighH = 158;
    int iLowS = 120;    int iHighS = 232;
    int iLowV = 117;    int iHighV = 255;
    int thresh = 100;

    //cvCreate trackbars in "Control" window
    cvCreateTrackbar( "Canny thresh:", "Control", &thresh, 255);
    cvCreateTrackbar("  RGB / HSB    ", "Control", &buttonHSV, 1);
    cvCreateTrackbar("Low Hue/Blue   ", "Control", &iLowH, 255); //Hue (0 - 179)
    cvCreateTrackbar("High Hue/Blue  ", "Control", &iHighH, 255);
    cvCreateTrackbar("Low Sat/Green  ", "Control", &iLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("High Sat/Green ", "Control", &iHighS, 255);
    cvCreateTrackbar("Low Bright/Red ", "Control", &iLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("High Bright/Red", "Control", &iHighV, 255);

    src_.buttonHSV = buttonHSV;
    src_.iLowH = iLowH;
    src_.iHighH = iHighH;
    src_.iLowS = iLowS;
    src_.iHighS = iHighS;
    src_.iLowV = iLowV;
    src_.iHighV = iHighV;
    src_.thresh = thresh;

    return src_;


}

vector<Point2f> color_detection(Mat imgOriginal, myThreshold test)
{
    Mat drawing, imgHSV, imgThresholded;;
    cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

    if(test.buttonHSV == 0){
        inRange(imgOriginal, Scalar(test.iLowH, test.iLowS, test.iLowV), Scalar(test.iHighH, test.iHighS, test.iHighV), imgThresholded); //Threshold the image
        //cout << "RGB = " << buttonHSV <<endl;
    }else{
        inRange(imgHSV, Scalar(test.iLowH, test.iLowS, test.iLowV), Scalar(test.iHighH, test.iHighS, test.iHighV), imgThresholded); //Threshold the image
        //cout << "HSV =" << buttonHSV <<endl;
    }

    //morphological opening (remove small objects from the foreground)
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    //morphological closing (fill small holes in the foreground)
    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    vector<Point2f> center;
    center = thresh_callback( 0, 0 , imgThresholded, test.thresh, drawing);

    namedWindow("Original", CV_WINDOW_FREERATIO);
    namedWindow("Thresholded Image", CV_WINDOW_FREERATIO);
    namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
    imshow("Thresholded Image", imgThresholded);
    imshow("Original", imgOriginal);

    return center;

}




vector<Point2f> thresh_callback(int, void*, Mat src, int thresh, Mat output)
{
  Mat canny_output,thresholded_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using canny
  //Canny( src, canny_output, thresh, thresh*2, 3 );
  //Detect edges using tresholded
  threshold(src,thresholded_output,thresh,255,THRESH_BINARY);
  /// Find contours
  findContours( thresholded_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  vector<vector<Point> > contours_poly (contours.size() );
  vector<Rect> boundRect (contours.size() );
  vector<Point2f> center (contours.size() );
  vector<float> radius (contours.size() );

  for(unsigned int i = 0; i < contours.size(); i ++)
  {
    approxPolyDP (Mat(contours[i]), contours_poly[i], 3, true);
    boundRect[i] = boundingRect (Mat (contours_poly[i]));
    minEnclosingCircle( (Mat) contours_poly[i], center[i], radius[i]);
  }

  /// Draw contours
    output = Mat::zeros( thresholded_output.size(), CV_8UC3 );
    for(unsigned int i = 0; i< contours.size(); i++ )
    {
        Scalar contour_color = Scalar( 255,0,0 );
        Scalar center_color = Scalar( 0,255,0 );
        drawContours( output, contours_poly, i, contour_color, 2, 8, hierarchy, 0, Point() );
        circle(output, center[i], 3,center_color, 2 ,8 ,0 );
    }



    imshow( "Contours", output );

    return center;

  /// Show in a window
}
