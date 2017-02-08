//#include <stdio.h> // from OpenCV introduction
#include <opencv2/opencv.hpp>
#include <string> // needed for setting std::strings and using to_string

using namespace cv;

int main(int argc, char** argv )
{
  // ------------------------------ TASK 1 ------------------------------
  // initialize the image object
  Mat image, image_gray;
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

  // convert image format to grayscale
  cvtColor(image, image_gray, CV_BGR2GRAY);


  // find the chessboard corners
  Size patternsize(10,7);
  std::vector<Point2f> centers;
  bool found;
  found = findChessboardCorners(image_gray, patternsize, centers);

  // refine corner locations with subpixel accuracy
  cornerSubPix(image_gray, centers, Size(5,5), Size(-1,-1), TermCriteria(3,30,0.1));

  // draw the corners in color on the original image
  drawChessboardCorners(image, patternsize, Mat(centers), found);

  // save image
  imwrite( "task1.jpg", image );

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
