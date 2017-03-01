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
  std::vector<Point2f>  centers_left,  centers4_left;
  std::vector<Point2f>  centers_right, centers4_right;

  Size                  patternsize(h,v);
  Mat                   image_left,      image_right,
                        image_left_gray, image_right_gray,
                        intrinsic_left,  intrinsic_right,
                        distortion_left, distortion_right,
                        R, T, E, F;

  namedWindow("Task 1 Left", CV_WINDOW_AUTOSIZE);
  namedWindow("Task 1 Right", CV_WINDOW_AUTOSIZE);
  moveWindow("Task 1 Left", 50, 50);
  moveWindow("Task 1 Right", 700, 50);

  // get left and right images
  path = "../images/checker/stereoL18.bmp";
  image_left = imread( path, 1 );
  path = "../images/checker/stereoR18.bmp";
  image_right = imread( path, 1 );

  // convert images format to grayscale
  cvtColor(image_left, image_left_gray, CV_BGR2GRAY);
  cvtColor(image_right, image_right_gray, CV_BGR2GRAY);
  // find the chessboard corners
  found = findChessboardCorners(image_left_gray,  patternsize, centers_left);
  found = findChessboardCorners(image_right_gray, patternsize, centers_right);
  // refine corner locations with subpixel accuracy
  cornerSubPix(image_left_gray,  centers_left,  Size(5,5), Size(-1,-1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 60, 0.001));
  cornerSubPix(image_right_gray, centers_right, Size(5,5), Size(-1,-1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 60, 0.001));

  // extract the 4 corner points
  centers4_left.push_back(centers_left[0]);
  centers4_left.push_back(centers_left[0+h-1]);
  centers4_left.push_back(centers_left[h*(v-1)]);
  centers4_left.push_back(centers_left[h*(v-1)+h-1]);
  centers4_right.push_back(centers_right[0]);
  centers4_right.push_back(centers_right[0+h-1]);
  centers4_right.push_back(centers_right[h*(v-1)]);
  centers4_right.push_back(centers_right[h*(v-1)+h-1]);

  // draw the 4 corner points
  Scalar color1 = Scalar(255,0,0);
  circle(image_left, centers4_left[0], 10, color1, 2);
  circle(image_left, centers4_left[1], 10, color1, 2);
  circle(image_left, centers4_left[2], 10, color1, 2);
  circle(image_left, centers4_left[3], 10, color1, 2);
  Scalar color2 = Scalar(0,0,255);
  circle(image_right, centers4_right[0], 10, color2, 2);
  circle(image_right, centers4_right[1], 10, color2, 2);
  circle(image_right, centers4_right[2], 10, color2, 2);
  circle(image_right, centers4_right[3], 10, color2, 2);

  // load existing calibration for left
  FileStorage fsr1("calibration_left_final.xml", FileStorage::READ);
  fsr1["intrinsic"] >> intrinsic_left;
  fsr1["distortion"] >> distortion_left;
  fsr1.release();
  // load existing calibration for right
  FileStorage fsr2("calibration_right_final.xml", FileStorage::READ);
  fsr2["intrinsic"] >> intrinsic_right;
  fsr2["distortion"] >> distortion_right;
  fsr2.release();
  // load existing stereo calibration parameters
  FileStorage fsr3("calibration_stereo_final.xml", FileStorage::READ);
  fsr3["R"] >> R;
  fsr3["T"] >> T;
  fsr3["E"] >> E;
  fsr3["F"] >> F;
  fsr3.release();

  // undistort the 4 corner points
  //undistortPoints(centers4_left, centers4_left, cameraMatrix, distCoeffs, InputArray R=noArray(), InputArray P=noArray())


  // undistortPoints() to undistort and rectify the 4 outermost points
  // perspectiveTransform() to calculate 3D information of 4 points
  // add circles to the 4 points on the image pairs & x,y,z information

  // in the writeup, put explanation describing the difference between key functions - above and beyond

  // display left and right images
  imshow("Task 1 Left", image_left);
  imshow("Task 1 Right", image_right);

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
