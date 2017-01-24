#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv )
{
  // provided code
  VideoCapture video(0);    // get a camera object
  if (!video.isOpened())
  {
    cout << "cannot open the video cam" << endl;
    return -1;
  }

  Mat frame;                // allocate an image buffer object
  namedWindow("Millard", CV_WINDOW_AUTOSIZE); // initialize a display window
  video >> frame;           // grab a frame from the video feed
  imshow("Millard", frame); // display the grabbed frame

  waitKey(0);

  return 0;
}
