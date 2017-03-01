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
  std::string   path;
  Mat           image_left, image_right;

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
