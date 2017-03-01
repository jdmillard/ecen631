#include <opencv2/opencv.hpp>
#include <string> // needed for setting std::strings and using to_string
#include <fstream>

using namespace cv;


int main(int argc, char** argv )
{


////////    ///     //////  //    //       //
   //      // //   //    // //   //      ////
   //     //   //  //       //  //         //
   //    //     //  //////  /////          //
   //    /////////       // //  //         //
   //    //     // //    // //   //        //
   //    //     //  //////  //    //     //////


  // initialize variables and windows

  int                   h = 10; // horizontal internal points
  int                   v = 7;  // vertical internal points

  bool                  found;

  std::string           path;
  std::vector<Point2f>  centers_left;
  std::vector<Point2f>  centers_right;

  Size                  patternsize(h,v);
  Mat                   image_left, image_right,
                        image_left_gray, image_right_gray;

  namedWindow("Task 1 Left", CV_WINDOW_AUTOSIZE);
  namedWindow("Task 1 Right", CV_WINDOW_AUTOSIZE);
  moveWindow("Task 1 Left", 50, 50);
  moveWindow("Task 1 Right", 700, 50);

  // get left and right images
  path = "../images/checker/stereoL18.bmp";
  image_left = imread( path, 1 );
  path = "../images/checker/stereoR18.bmp";
  image_right = imread( path, 1 );

  // display left and right images
  imshow("Task 1 Left", image_left);
  imshow("Task 1 Right", image_right);

  // convert images format to grayscale
  cvtColor(image_left, image_left_gray, CV_BGR2GRAY);
  cvtColor(image_right, image_right_gray, CV_BGR2GRAY);
  // find the chessboard corners
  found = findChessboardCorners(image_left_gray, patternsize, centers_left);
  found = findChessboardCorners(image_left_gray, patternsize, centers_right);
  // refine corner locations with subpixel accuracy
  cornerSubPix(image_left_gray,  centers_left,  Size(5,5), Size(-1,-1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 60, 0.001));
  cornerSubPix(image_right_gray, centers_right, Size(5,5), Size(-1,-1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 60, 0.001));



  // undistortPoints() to undistort and rectify the 4 outermost points
  // perspectiveTransform() to calculate 3D information of 4 points
  // add circles to the 4 points on the image pairs & x,y,z information

  // in the writeup, put explanation describing the difference between key functions - above and beyond

  // wait for a new key input
  int key = waitKey();
  if (key == 110)
  {
    // the 'n' (next) key was pressed
  }
  else if (key == 27)
  {
    // the 'esc' key was pressed, end application
    std::cout << "terminating" << std::endl;
    return -1;
  }


  printf("finished \n");
  return 0;
}
