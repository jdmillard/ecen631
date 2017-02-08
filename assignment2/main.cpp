//#include <stdio.h> // from OpenCV introduction
#include <opencv2/opencv.hpp>
#include <string> // needed for setting std::strings and using to_string

using namespace cv;

int main(int argc, char** argv )
{
  // set save bool
  bool saving = true;


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

  if (saving)
  {
    // save image
    imwrite( "task1.jpg", image );
  }

  // display image in an opencv window
  imshow("Task 1", image);

  // wait for a new key input
  int key = waitKey();
  if (key == 110)
  {
    // the 'n' (next) key was pressed, increment file
    std::cout << "moving on to task 2" << std::endl;
  }
  else if (key == 27)
  {
    // the 'esc' key was pressed, end application
    std::cout << "terminating" << std::endl;
    return -1;
  }

  // ------------------------------ TASK 2 ------------------------------
  // create OpenCV gui window
  namedWindow("Task 2", CV_WINDOW_AUTOSIZE); // initialize a display window

  // cycle through each calibration image
  std::string n_img_str;
  int n_images = 40;
  for (int i = 1; i<=n_images; i++)
  {
    n_img_str = std::to_string(i);
    path = "../images/AR" + n_img_str + ".jpg";
    image = imread( path, 1 );

    // test image validity
    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    // display the current image
    imshow("Task 2", image);


    // wait for a new key input
    key = waitKey();
    if (key == 110)
    {
      // the 'n' (next) key was pressed, increment file
      std::cout << "next image" << std::endl;
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
