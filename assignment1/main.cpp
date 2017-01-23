#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv )
{
  // provided code
  VideoCapture video(0);    // get a camera object
  Mat frame;                // allocate an image buffer object
  namedWindow("Millard", CV_WINDOW_AUTOSIZE); // initialize a display window
  video >> frame;           // grab a frame from the video feed
  imshow("Millard", frame); // display the grabbed frame

  waitKey(0);

  return 0;
}
