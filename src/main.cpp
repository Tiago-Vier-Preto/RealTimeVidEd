#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp" 
#include <iostream> 

using namespace cv;
using namespace std;

const int alpha_slider_max = 30;
int slider_value;
int value_trackbar = 0;
int camera = 0;

bool record = false;
bool applyGaussianBlur = false;

void on_trackbar(int, void*);

int main(void)
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
    slider_value = 0;
    char TrackbarName[50];
    snprintf(TrackbarName, sizeof(TrackbarName), "Alpha x %d", alpha_slider_max);
    createTrackbar(TrackbarName, "Output Video", &slider_value, alpha_slider_max, on_trackbar);

    VideoWriter writer;
    int frame_width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));
    int fps = static_cast<int>(cap.get(CAP_PROP_FPS));

    Mat frame, output_frame;
    while (true)
    {
        cap >> frame;
        if (frame.empty())
        {
            std::cerr << "Error: Could not read frame" << std::endl;
            break;
        }
        frame.copyTo(output_frame); 

        if (applyGaussianBlur)
        {
            int k_size = value_trackbar * 2 + 1;
            GaussianBlur(frame, output_frame, Size(k_size, k_size), 0);
        } 

        if (record) 
        {
            writer.write(output_frame); 
        }

        imshow("Input Video", frame);
        imshow("Output Video", output_frame);

        int key = waitKey(10);
        if (key == 82 || key == 114) // Record key (R or r)
        {
            if (record) 
            {
                writer.release(); 
                cout << "Recording stopped." << endl;
            }
            else 
            {
                writer.open("output_video.mp4", VideoWriter::fourcc('H', '2', '6', '4'), fps, Size(frame_width, frame_height));
                if (!writer.isOpened())
                {
                    std::cerr << "Error: Could not open VideoWriter" << std::endl;
                    return -1;
                }
                cout << "Recording started." << endl;
            }
            record = !record;
        }

        if (key == 71 || key == 103) // Gaussian Blur key (G or g)
        {
            applyGaussianBlur = !applyGaussianBlur;
        }

        if (key == 27 || getWindowProperty("Input Video", WND_PROP_VISIBLE) < 1 || getWindowProperty("Output Video", WND_PROP_VISIBLE) < 1)
        {
            break;
        }
    }
    
    cap.release();
    writer.release();
    destroyAllWindows();
    return 0;
}

void on_trackbar(int, void*)
{
    value_trackbar = slider_value;
}
