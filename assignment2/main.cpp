//#include <stdio.h> // from OpenCV introduction
#include <opencv2/opencv.hpp>
#include <string> // needed for setting std::strings and using to_string

using namespace cv;

int main(int argc, char** argv )
{
  // ------------------------------ TASK 1 ------------------------------
  // initialize the image object
  Mat image;
  // create OpenCV gui window
  namedWindow("Task 1", CV_WINDOW_AUTOSIZE); // initialize a display window

  // import a single calibration image
  std::string path;
  path = "../images/AR1.jpg";
  image = imread( path, 1 );

  // test image validity
  if ( !image.data )
  {
      printf("No image data \n");
      return -1;
  }

  // display image in an opencv window
  imshow("Task 1", image);



  // wait for a new key input
  int key = waitKey();
  if (key == 110)
  {
    // the 'n' (next) key was pressed, increment file
    std::cout << "no functionality yet" << std::endl;
  }
  else if (key == 27)
  {
    // the 'esc' key was pressed, end application
    std::cout << "terminating" << std::endl;
  }



  return 0;
}
