 
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <thread>
 
using namespace cv;
using namespace std;
 
const int alpha_slider_max = 100;
int alpha_slider;
int camera = 0;
double alpha;

void on_trackbar( int, void* );
 
int main( void )
{
    namedWindow("Input Video", WINDOW_AUTOSIZE);
    namedWindow("Output Video", WINDOW_AUTOSIZE);

    VideoCapture cap(camera);
    if (!cap.isOpened())
    {
        std::cerr << "Error: Could not open camera" << std::endl;
        return -1;
    }

    // Trackbar 
    alpha_slider = 0;
    char TrackbarName[50];
    snprintf( TrackbarName, sizeof(TrackbarName), "Alpha x %d", alpha_slider_max );
    createTrackbar( TrackbarName, "Output Video", &alpha_slider, alpha_slider_max, on_trackbar );

    Mat frame;
    while (true)
    {
        cap >> frame;
        if (frame.empty())
        {
            std::cerr << "Error: Could not read frame" << std::endl;
            break;
        }

        imshow("Input Video", frame);
        imshow("Output Video", frame);
        if (waitKey(10) == 27 || getWindowProperty("Input Video", WND_PROP_VISIBLE) < 1 || getWindowProperty("Output Video", WND_PROP_VISIBLE) < 1)
        {
            break;
        }
    }
    
    cap.release();
    destroyAllWindows();
    return 0;
}

void on_trackbar( int, void* )
{
   alpha = (double) alpha_slider/alpha_slider_max;
}