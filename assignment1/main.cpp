//#include <stdio.h> // from OpenCV introduction, not needed for this
#include <opencv2/opencv.hpp>
//#include <iostream> // not needed

using namespace cv;
//using namespace std; // needed for cout/endl (not needed if std:: is used)

int main(int argc, char** argv )
{
  VideoCapture video(0);    // get a camera object

  // attempt video access, and print relevant information to screen
  if (!video.isOpened())
  {
    std::cout << "cannot access video" << std::endl;
    return -1;
  }
  std::cout << "video access successful" << std::endl;
  double imgW = video.get(CAP_PROP_FRAME_WIDTH);  //get width of the video
  double imgH = video.get(CAP_PROP_FRAME_HEIGHT); //get height of the video
  std::cout << "video dimensions : " << imgW << " x " << imgH << std::endl;


  Mat frame;                  // allocate an image buffer object
  namedWindow("Millard", CV_WINDOW_AUTOSIZE); // initialize a display window


  // perform loop of video feed updates
  while (1)
  {
    video >> frame;           // grab a frame from the video feed
    imshow("Millard", frame); // display the grabbed frame
    waitKey(1);               // wait for a few ms
  }


  return 0;
}
