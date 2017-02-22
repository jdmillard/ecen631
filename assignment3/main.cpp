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


  // individual camera calibration (loaded from xml when recalibrate=false)
  // initialize variables
  Mat         image_left, image_right, image_gray, image_mod;
  Mat intrinsic_left;
  Mat distortion_left;
  Mat intrinsic_right;
  Mat distortion_right;
  std::string path, n_img_str;
  int         n_img;
  int         h = 10; // horizontal internal points
  int         v = 7;  // vertical internal points
  float       square = 3.88;  // width of chessboard squares
  Size        patternsize(h,v);
  std::vector<Point2f> centers;
  bool        found;
  bool        recalibrate = true;

  if (recalibrate)
  {

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
        cornerSubPix(image_gray, centers, Size(5,5), Size(-1,-1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 60, 0.001));
        // add the resulting vector of points to the overall vector
        img_points_left.push_back(centers);
        obj_points_left.push_back(obj_set);
        // draw the corners in color on the original image
        image_mod = image_left.clone();
        drawChessboardCorners(image_mod, patternsize, Mat(centers), found);
        // display the current image
        imshow("Task 1 Left", image_mod);
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
        cornerSubPix(image_gray, centers, Size(5,5), Size(-1,-1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 60, 0.001));
        // add the resulting vector of points to the overall vector
        img_points_right.push_back(centers);
        obj_points_right.push_back(obj_set);
        // draw the corners in color on the original image
        image_mod = image_right.clone();
        drawChessboardCorners(image_mod, patternsize, Mat(centers), found);
        // display the current image
        imshow("Task 1 Right", image_mod);
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


    } // end of image looping


    // calibrate the left camera
    std::vector<Mat> rvecs;
    std::vector<Mat> tvecs;
    // now use the overall vectors to get the camera calibration
    std::cout << "calculating left calibration..." << std::endl;
    calibrateCamera(obj_points_left, img_points_left, image_gray.size(), intrinsic_left, distortion_left, rvecs, tvecs);
    // write the calibration data to "calibration_left.xml"
    FileStorage fsw1("calibration_left.xml", FileStorage::WRITE);
    fsw1 << "intrinsic" << intrinsic_left << "distortion" << distortion_left;
    fsw1.release();


    // calibrate the right camera
    // now use the overall vectors to get the camera calibration
    std::cout << "calculating right calibration..." << std::endl;
    calibrateCamera(obj_points_right, img_points_right, image_gray.size(), intrinsic_right, distortion_right, rvecs, tvecs);
    // write the calibration data to "calibration_right.xml"
    FileStorage fsw2("calibration_right.xml", FileStorage::WRITE);
    fsw2 << "intrinsic" << intrinsic_right << "distortion" << distortion_right;
    fsw2.release();


  } // end of if recalibrate check
  else
  {
    // use existing calibration for left
    FileStorage fsr1("calibration_left_final.xml", FileStorage::READ);
    fsr1["intrinsic"] >> intrinsic_left;
    fsr1["distortion"] >> distortion_left;
    fsr1.release();
    // use existing calibration for right
    FileStorage fsr2("calibration_right_final.xml", FileStorage::READ);
    fsr2["intrinsic"] >> intrinsic_right;
    fsr2["distortion"] >> distortion_right;
    fsr2.release();

  }


////////    ///     //////  //    //     ///////
   //      // //   //    // //   //     //     //
   //     //   //  //       //  //             //
   //    //     //  //////  /////        ///////
   //    /////////       // //  //      //
   //    //     // //    // //   //     //
   //    //     //  //////  //    //    /////////


  // stereo calibration


  namedWindow("Task 2 Left", CV_WINDOW_AUTOSIZE);
  namedWindow("Task 2 Right", CV_WINDOW_AUTOSIZE);
  moveWindow("Task 2 Left", 50, 50);
  moveWindow("Task 2 Right", 700, 50);

  // initialize vector of vectors of 2-D floating points
  std::vector<std::vector<Point2f>> s_img_points_left;
  std::vector<std::vector<Point2f>> s_img_points_right;
  std::vector<std::vector<Point3f>> s_obj_points;
  std::vector<Point3f> s_obj_set;
  Point3f s_p_cur;

  // s_obj_set is set of world frame checker points (0,0,0), (1,0,0) ... (n,m,0)
  // each image will have a corresponding one, but they are all identical
  // based on the way the calibrate camera defines the world frames

  // create a vector of 3d points
  for (int y=0; y<v; y++)
  {
    for (int x=0; x<h; x++)
    {
      // generate the current point in accurate checkerboard scale
      s_p_cur.x = x*square;
      s_p_cur.y = y*square;
      s_p_cur.z = 0.0f;

      s_obj_set.push_back(s_p_cur);
    }
  }


  // loop through left and right images
  for (n_img = 0; n_img <= 59; n_img++)
  {
    std::cout << n_img << std::endl;
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
    path = "../images/stereo/stereoL" + n_img_str + ".bmp";
    image_left = imread( path, 1 );
    path = "../images/stereo/stereoR" + n_img_str + ".bmp";
    image_right = imread( path, 1 );


    // test left image validity (of both left and right) and display
    if ( (!image_left.data) || (!image_right.data) )
    {
      printf("image pair omitted \n");
      //return -1;
    }
    else
    {
      // find chessboard corners of left stereo image
      // convert image format to grayscale
      cvtColor(image_left, image_gray, CV_BGR2GRAY);
      // find the chessboard corners
      found = findChessboardCorners(image_gray, patternsize, centers);
      // refine corner locations with subpixel accuracy
      cornerSubPix(image_gray, centers, Size(5,5), Size(-1,-1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 60, 0.001));
      // add the resulting vector of points to the overall vector
      s_img_points_left.push_back(centers);
      // draw the corners in color on the original image
      image_mod = image_left.clone();
      drawChessboardCorners(image_mod, patternsize, Mat(centers), found);
      // display the current image
      imshow("Task 2 Left", image_mod);


      // find chessboard corners of right stereo image
      // convert image format to grayscale
      cvtColor(image_right, image_gray, CV_BGR2GRAY);
      // find the chessboard corners
      found = findChessboardCorners(image_gray, patternsize, centers);
      // refine corner locations with subpixel accuracy
      cornerSubPix(image_gray, centers, Size(5,5), Size(-1,-1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 60, 0.001));
      // add the resulting vector of points to the overall vector
      s_img_points_right.push_back(centers);
      // draw the corners in color on the original image
      image_mod = image_right.clone();
      drawChessboardCorners(image_mod, patternsize, Mat(centers), found);
      // display the current image
      imshow("Task 2 Right", image_mod);


      // add the corresponding object points that are seen in both images
      s_obj_points.push_back(s_obj_set);


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

  } // end of loop through images


  // perform stereo calibration


  Mat R, T, E, F;
  std::cout << "calculating stereo calibration..." << std::endl;
  double rms = stereoCalibrate( s_obj_points,
                                s_img_points_left,  s_img_points_right,
                                intrinsic_left,     distortion_left,
                                intrinsic_right,    distortion_right,
                                image_gray.size(),  R,  T,  E,  F,
                                CV_CALIB_FIX_INTRINSIC,
                                TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 100, 1e-5) );

  std::cout << R << std::endl;
  std::cout << T << std::endl;
  std::cout << E << std::endl;
  std::cout << F << std::endl;
  // write the calibration data to "calibration_stereo.xml"
  FileStorage s_fsw("calibration_stereo.xml", FileStorage::WRITE);
  s_fsw << "R" << R << "T" << T << "E" << E << "F" << F;
  s_fsw.release();






  printf("finished \n");
  return 0;
}
