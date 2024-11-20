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

void on_trackbar(int, void*);

enum
{
    ORIGINAL,
    GAUSSIAN_BLUR,
    CANNY_EDGE_DETECTION,
    SOBEL_EDGE_DETECTION,
    BRIGHTNESS,
    CONTRAST,
    NEGATIVE
};

int selectEfect = ORIGINAL;

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

        int k_size;

        switch (selectEfect)
        {
        case ORIGINAL:
            frame.copyTo(output_frame);
            break;
        case GAUSSIAN_BLUR:
            k_size = 2 * value_trackbar + 1;
            GaussianBlur(frame, output_frame, Size(k_size, k_size), 0, 0);
            break;
        case CANNY_EDGE_DETECTION:
            Canny(frame, output_frame, 100, 200);
            break;
        case SOBEL_EDGE_DETECTION:
            Sobel(frame, output_frame, CV_8U, 1, 1);
            break;
        case BRIGHTNESS:
            frame.convertTo(output_frame, -1, 1, value_trackbar);
            break;
        case CONTRAST:
            frame.convertTo(output_frame, -1, value_trackbar, 0);
            break;
        case NEGATIVE:
            frame.convertTo(output_frame, -1, -1, 255);
            break;
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
            selectEfect = GAUSSIAN_BLUR;
        }

        if (key == 69 || key == 101) // Canny Edge Detection key (E or e)
        {
            selectEfect = CANNY_EDGE_DETECTION;
        }

        if (key == 83 || key == 115) // Sobel Edge Detection key (S or s)
        {
            selectEfect = SOBEL_EDGE_DETECTION;
        }
        
        if (key == 66 || key == 98) // Brightness key (B or b)
        {
            selectEfect = BRIGHTNESS;
        }

        if (key == 67 || key == 99) // Contrast key (C or c)
        {
            selectEfect = CONTRAST;
        }

        if (key == 78 || key == 110) // Negative key (N or n)
        {
            selectEfect = NEGATIVE;
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
