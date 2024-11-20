 
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
 
void displayInputVideo();
void displayOutputVideo();
void on_trackbar( int, void* );
 
int main( void )
{
    
    thread t1(displayInputVideo);
    thread t2(displayOutputVideo);

    t1.join();
    t2.join();
    
    destroyAllWindows();
    return 0;
}

void displayInputVideo()
{
    namedWindow("Input Video", WINDOW_AUTOSIZE);

    VideoCapture cap(camera);
    if (!cap.isOpened())
    {
        std::cerr << "Error: Could not open camera" << std::endl;
        return;
    }

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
        if (waitKey(10) == 27 || getWindowProperty("Input Video", WND_PROP_VISIBLE) < 1)
        {
            break;
        }
    }
    
    cap.release();
    destroyWindow("Input Video");
}

void displayOutputVideo()
{
    namedWindow("Output Video", WINDOW_AUTOSIZE);

    VideoCapture cap(camera);
    if (!cap.isOpened())
    {
        std::cerr << "Error: Could not open camera" << std::endl;
        return;
    }

    Mat frame;

    alpha_slider = 0;
    char TrackbarName[50];
    snprintf( TrackbarName, sizeof(TrackbarName), "Alpha x %d", alpha_slider_max );
    createTrackbar( TrackbarName, "Output Video", &alpha_slider, alpha_slider_max, on_trackbar );

    while (true)
    {
        cap >> frame;
        if (frame.empty())
        {
            std::cerr << "Error: Could not read frame" << std::endl;
            break;
        }

        Mat output;
        addWeighted(frame, alpha, frame, 1.0 - alpha, 0.0, output);
        imshow("Output Video", output);
        if (waitKey(10) == 27 || getWindowProperty("Output Video", WND_PROP_VISIBLE) < 1)
        {
            break;
        }
    }

    cap.release();
    destroyWindow("Output Video");
}

void on_trackbar( int, void* )
{
   alpha = (double) alpha_slider/alpha_slider_max;
}