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
  Mat frame_out;              // allocate an image buffer object
  namedWindow("Task 1", CV_WINDOW_AUTOSIZE); // initialize a display window
  namedWindow("Task 2", CV_WINDOW_AUTOSIZE); // initialize a display window

  // there are various functions to implement, 0 is no function at all
  int type = 0;
  int last_type = 5;

  // perform loop of video feed updates
  while (1)
  {
    video >> frame;           // grab a frame from the video feed

    if (type==0)      // STANDARD VIDEO
    {
      // assign output to the standard frame by value using .clone()
      frame_out = frame.clone();
    }
    else if (type==1) // FUNCTION 1 - THRESHOLD
    {
      threshold(frame, frame_out, 127, 255, THRESH_BINARY);
    }
    else if (type==2) // FUNCTION 2 - CANNY EDGE DETECTION
    {
      Canny(frame, frame_out, 175, 220);
    }
    else if (type==3) // FUNCTION 3 - CORNER
    {
      //std::cout << "3" << std::endl;
      threshold(frame, frame_out, 127, 255,THRESH_BINARY);
    }
    else if (type==4) // FUNCTION 4
    {
      //std::cout << "4" << std::endl;
      threshold(frame, frame_out, 127, 255,THRESH_BINARY);
    }
    else if (type==5) // FUNCTION 5
    {
      //std::cout << "5" << std::endl;
      threshold(frame, frame_out, 127, 255,THRESH_BINARY);
    }



    //threshold(frame, frame_out, 127, 255,THRESH_BINARY);


    imshow("Task 1", frame);      // display the grabbed frame
    imshow("Task 2", frame_out);  // display the grabbed frame




    // wait for a few ms, listening for specific keys to determine rotation
    int key = waitKey(30);
    if (key == 110)
    {
      // the 'n' (next) key was pressed
      std::cout << "next" << std::endl;
      if (type == last_type)
      {
        type = 0;
      }
      else
      {
        type++;
      }

      std::cout << type << std::endl;
    }
    else if (key == 112)
    {
      // the 'p' (previous) key was pressed
      std::cout << "previous" << std::endl;
      if (type == 0)
      {
        type = last_type;
      }
      else
      {
        type--;
      }
      std::cout << type << std::endl;
    }

  }


  return 0;
}
