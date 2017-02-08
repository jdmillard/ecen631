//#include <stdio.h> // from OpenCV introduction
#include <opencv2/opencv.hpp>
#include <string> // needed for setting std::strings and using to_string

using namespace cv;

int main(int argc, char** argv )
{
  // set save bool
  bool saving = true;
  int h = 10; // horizontal internal points
  int v = 7;  // vertical internal points


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
  Size patternsize(h,v);
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
  //namedWindow("Task 2", CV_WINDOW_AUTOSIZE); // initialize a display window

  // initialize vector of vectors of 2-D floating points
  std::vector<std::vector<Point2f>> img_points;
  std::vector<std::vector<Point3f>> obj_points;
  std::vector<Point3f> obj_set;
  Point3f p_cur;

  // obj_set is set of world frame checker points (0,0,0), (1,0,0) ... (n,m,0)
  // each image will have a corresponding one, but they are all identical
  // based on the way the calibrate camera defines the world frames

  // create a vector of 3d points corresponding
  for (int x=0; x<h; x++)
  {
    for (int y=0; y<v; y++)
    {
      // generate the current point
      p_cur.x = x;
      p_cur.y = y;
      p_cur.z = 0.0f;

      obj_set.push_back(p_cur);
    }
  }


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

    // convert image format to grayscale
    cvtColor(image, image_gray, CV_BGR2GRAY);

    // find the chessboard corners
    found = findChessboardCorners(image_gray, patternsize, centers);

    // refine corner locations with subpixel accuracy
    cornerSubPix(image_gray, centers, Size(5,5), Size(-1,-1), TermCriteria(3,30,0.1));

    // add the resulting vector of points to the overall vector
    img_points.push_back(centers);
    obj_points.push_back(obj_set);

    // draw the corners in color on the original image
    drawChessboardCorners(image, patternsize, Mat(centers), found);

    // display the current image
    //imshow("Task 2", image);
  }


  // now use the overall vectors to get the camera calibration
  ret, mtx, dist, rvecs, tvecs = calibrateCamera(obj_points, img_points, image.shape[::-1], None, None);

  // wait for a new key input
  key = waitKey();
  if (key == 110)
  {
    // the 'n' (next) key was pressed, increment image
  }
  else if (key == 27)
  {
    // the 'esc' key was pressed, end application
    std::cout << "terminating" << std::endl;
    return -1;
  }



  return 0;
}
