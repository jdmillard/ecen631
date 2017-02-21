#include <opencv2/opencv.hpp>
#include <string> // needed for setting std::strings and using to_string
#include <fstream>

using namespace cv;

int main(int argc, char** argv )
{


  // for Task 1, bring in the images and display them so we know that they work
  // only bring in the relevant ones

  Mat image;
  std::string path, n_img_str;
  int n_img;

  namedWindow("Task 1", CV_WINDOW_AUTOSIZE); // initialize a display window

  // for loop here
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
    path = "../images/left/leftL" + n_img_str + ".bmp";
    std::cout << path << std::endl;

    // need to do a path check for images which have been omitted



    // load current image
    image = imread( path, 1 );



    // test image validity
    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }


    imshow("Task 1", image);




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


return 0;
}
