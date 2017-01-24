//#include <stdio.h> // from OpenCV introduction, not needed for this
#include <opencv2/opencv.hpp>
//#include <iostream> // not needed

using namespace cv;
//using namespace std; // needed for cout/endl (not needed if std:: is used)

int main(int argc, char** argv )
{
  VideoCapture video(0);    // get a camera object
  VideoWriter VOut;         // Create a video write object.
  // initialize video write object
  VOut.open("VideoOut.avi", CV_FOURCC('M', 'P', 'E', 'G') , 30, Size(640, 480), 1);
  // alternative syntax if missing codec
  //VOut.open("VideoOut.avi", -1 , 30, Size(640, 480), 1);


  // attempt video access, and print relevant information to screen
  if (!video.isOpened())
  {
    std::cout << "cannot access video, see build readme" << std::endl;
    return -1;
  }
  std::cout << "video access successful" << std::endl;
  double imgW = video.get(CAP_PROP_FRAME_WIDTH);  //get width of the video
  double imgH = video.get(CAP_PROP_FRAME_HEIGHT); //get height of the video
  std::cout << "video dimensions : " << imgW << " x " << imgH << std::endl;


  // initialize data objects of type cv::Mat
  Mat frame, frame_out, frame_d1, gray;


  // create OpenCV gui windows
  namedWindow("Task 1", CV_WINDOW_AUTOSIZE); // initialize a display window
  namedWindow("Task 2", CV_WINDOW_AUTOSIZE); // initialize a display window


  // there are various functions to implement, use integers to represent them
  int type = 0;       // starting type (plain video)
  int last_type = 5;  // number of functions to cycle through


  // perform loop to update the frames from the camera
  video >> frame; // update once here for the sake of the *_d1 frame
  while (1)
  {
    // update frames
    frame_d1 = frame.clone();
    video >> frame;


    // perform a function based on "type" variable
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
      cvtColor(frame_out, frame_out, CV_GRAY2BGR);
    }
    else if (type==3) // FUNCTION 3 - CORNER
    {
      // convert source image to gray using CV_BGR2GRAY
      cvtColor(frame, gray, CV_BGR2GRAY);
      // format frame_out as zeros using CV_32FC1
      frame_out = Mat::zeros(frame.size(), CV_32FC1);
      cornerHarris(gray, frame_out, 7, 5, 0.05);

      normalize(frame_out, frame_out, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
      convertScaleAbs(frame_out, frame_out);
    }
    else if (type==4) // FUNCTION 4 - HOUGH LINE DETECTION
    {
      // first detect the edges
      Canny(frame, frame_out, 175, 220);

      // initialize a vector of cv::Vec2f lines (put outside loop)
      std::vector<Vec2f> lines;
      HoughLines(frame_out, lines, 1, CV_PI/180, 150, 0, 0);

      // reset frame_out to original feed
      frame_out = frame.clone();

      // cycle through each line, find two defining points, and draw on image
      for( size_t i = 0; i < lines.size(); i++ )
        {
          float rho = lines[i][0], theta = lines[i][1];
          Point pt1, pt2;
          double a = cos(theta), b = sin(theta);
          double x0 = a*rho, y0 = b*rho;
          pt1.x = cvRound(x0 + 1000*(-b));
          pt1.y = cvRound(y0 + 1000*(a));
          pt2.x = cvRound(x0 - 1000*(-b));
          pt2.y = cvRound(y0 - 1000*(a));
          line( frame_out, pt1, pt2, Scalar(255,0,0), 3, CV_AA);
        }

    }
    else if (type==5) // FUNCTION 5 - DIFFERENCE IMAGES
    {
      // frame is current frame
      // frame_d1 is delayed 1 iteration
      absdiff(frame, frame_d1, frame_out);
    }


    imshow("Task 1", frame);      // display the grabbed frame
    imshow("Task 2", frame_out);  // display the grabbed frame
    VOut << frame_out;            // save frame to video file


    // wait for a few ms, listening for specific keys to determine rotation
    int key = waitKey(30);
    if (key == 110)
    {
      // the 'n' (next) key was pressed, increment "type" variable
      if (type == last_type)
      {
        type = 0;
      }
      else
      {
        type++;
      }
    }
    else if (key == 112)
    {
      // the 'p' (previous) key was pressed, decrement "type" variable
      if (type == 0)
      {
        type = last_type;
      }
      else
      {
        type--;
      }
    }
    else if (key == 27)
    {
      // the 'esc' key was pressed, end application
      std::cout << "terminating" << std::endl;
      break;
    }


  } // end of while loop


  return 0;
}
