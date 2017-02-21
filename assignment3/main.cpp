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


  // initialize variables
  Mat         image_left, image_right, image_gray;
  std::string path, n_img_str;
  int         n_img;
  int         h = 10; // horizontal internal points
  int         v = 7;  // vertical internal points
  float       square = 3.88;  // width of chessboard squares
  Size        patternsize(h,v);
  std::vector<Point2f> centers;
  bool        found;

  namedWindow("Task 1 Left", CV_WINDOW_AUTOSIZE);
  namedWindow("Task 1 Right", CV_WINDOW_AUTOSIZE);
  moveWindow("Task 1 Left", 50, 50);
  moveWindow("Task 1 Right", 700, 50);

  // initialize vector of vectors of 2-D floating points
  std::vector<std::vector<Point2f>> img_points_left;
  std::vector<std::vector<Point2f>> img_points_right;
  std::vector<std::vector<Point3f>> obj_points_left;
  std::vector<std::vector<Point3f>> obj_points_right;
  std::vector<Point3f> obj_set;
  Point3f p_cur;

  // obj_set is set of world frame checker points (0,0,0), (1,0,0) ... (n,m,0)
  // each image will have a corresponding one, but they are all identical
  // based on the way the calibrate camera defines the world frames

  // create a vector of 3d points
  for (int y=0; y<v; y++)
  {
    for (int x=0; x<h; x++)
    {
      // generate the current point in accurate checkerboard scale
      p_cur.x = x*square;
      p_cur.y = y*square;
      p_cur.z = 0.0f;

      obj_set.push_back(p_cur);
    }
  }




  // loop through left and right images
  for (n_img = 0; n_img <= 59; n_img++)
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
      printf("left image omitted \n");
      //return -1;
    }
    else
    {
      // convert image format to grayscale
      cvtColor(image_left, image_gray, CV_BGR2GRAY);

      // find the chessboard corners
      found = findChessboardCorners(image_gray, patternsize, centers);

      // refine corner locations with subpixel accuracy
      //cornerSubPix(image_gray, centers, Size(5,5), Size(-1,-1), TermCriteria(3,30,0.1));

      // add the resulting vector of points to the overall vector
      img_points_left.push_back(centers);
      obj_points_left.push_back(obj_set);

      // draw the corners in color on the original image
      drawChessboardCorners(image_left, patternsize, Mat(centers), found);

      // display the current image
      imshow("Task 1 Left", image_left);
    }


    // test right image validity and display
    if ( !image_right.data )
    {
      printf("right image omitted \n");
      //return -1;
    }
    else
    {
      std::cout << n_img << std::endl;
      // convert image format to grayscale
      cvtColor(image_right, image_gray, CV_BGR2GRAY);

      // find the chessboard corners
      found = findChessboardCorners(image_gray, patternsize, centers);

      // refine corner locations with subpixel accuracy
      //cornerSubPix(image_gray, centers, Size(5,5), Size(-1,-1), TermCriteria(3,30,0.1));

      // add the resulting vector of points to the overall vector
      img_points_right.push_back(centers);
      obj_points_right.push_back(obj_set);

      // draw the corners in color on the original image
      drawChessboardCorners(image_right, patternsize, Mat(centers), found);

      // display the current image
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

printf("finished \n");
return 0;
}
