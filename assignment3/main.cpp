#include <opencv2/opencv.hpp>
#include <string> // needed for setting std::strings and using to_string
#include <fstream>

using namespace cv;

int main(int argc, char** argv )
{


  // for Task 1, bring in the images and display them so we know that they work
  // only bring in the relevant ones

  Mat         image_left, image_right;
  std::string path, n_img_str;
  int         n_img;

  namedWindow("Task 1 Left", CV_WINDOW_AUTOSIZE);
  namedWindow("Task 1 Right", CV_WINDOW_AUTOSIZE);

  // loop through left and right images
  for (n_img = 0; n_img < 65; n_img++)
  {


    // convert n_img to string, accounting for the 0 in front
    if (n_img < 10)
    {
      n_img_str = "0" + std::to_string(n_img);
    }
    else
    {
      n_img_str = std::to_string(n_img);
    }


    // get left and right images
    path = "../images/left/leftL" + n_img_str + ".bmp";
    image_left = imread( path, 1 );
    path = "../images/right/rightR" + n_img_str + ".bmp";
    image_right = imread( path, 1 );


    // test left image validity and display
    if ( !image_left.data )
    {
      printf("no left image data \n");
      //return -1;
    }
    else
    {
      imshow("Task 1 Left", image_left);
    }


    // test right image validity and display
    if ( !image_right.data )
    {
      printf("no right image data \n");
      //return -1;
    }
    else
    {
      imshow("Task 1 Right", image_right);
    }


    // wait for a new key input
    int key = waitKey();
    if (key == 110)
    {
      // the 'n' (next) key was pressed, increment file
      //std::cout << "next task" << std::endl;
    }
    else if (key == 27)
    {
      // the 'esc' key was pressed, end application
      std::cout << "terminating" << std::endl;
      return -1;
    }


  }

printf("reached end \n");
return 0;
}
