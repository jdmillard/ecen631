#include <opencv2/opencv.hpp>
#include <string> // needed for setting std::strings and using to_string
#include <fstream>

using namespace cv;

void drawEpiLines(Mat img, const std::vector<Point3f>& lines) {
    for (auto&& ell : lines) {
        line(img,
            Point(0, -ell.z / ell.y),
            Point(img.cols, -(ell.z + ell.x*img.cols)/ell.y),
            Scalar(255, 0, 0));
    }
}


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
  bool        recalibrate = false;

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
  Mat R, T, E, F;
  recalibrate = false;
  if (recalibrate)
  {
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
    std::cout << "calculating stereo calibration..." << std::endl;
    double rms = stereoCalibrate( s_obj_points,
                                  s_img_points_left,  s_img_points_right,
                                  intrinsic_left,     distortion_left,
                                  intrinsic_right,    distortion_right,
                                  image_gray.size(),  R,  T,  E,  F,
                                  CV_CALIB_FIX_INTRINSIC,
                                  TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 100, 1e-5) );

    // write the calibration data to "calibration_stereo.xml"
    FileStorage s_fsw("calibration_stereo.xml", FileStorage::WRITE);
    s_fsw << "R" << R << "T" << T << "E" << E << "F" << F;
    s_fsw.release();
  }
  else
  {
    // use existing calibration for stereo
    //FileStorage s_fsr("calib_stereo.yaml", FileStorage::READ);
    FileStorage s_fsr("calibration_stereo_final.xml", FileStorage::READ);
    s_fsr["R"] >> R;
    s_fsr["T"] >> T; // for Parker's calibration, make "T" > "t"
    s_fsr["E"] >> E;
    s_fsr["F"] >> F;
    s_fsr.release();
  }


////////    ///     //////  //    //     ///////
   //      // //   //    // //   //     //     //
   //     //   //  //       //  //             //
   //    //     //  //////  /////        ///////
   //    /////////       // //  //             //
   //    //     // //    // //   //     //     //
   //    //     //  //////  //    //     ///////


  // epipolar lines
  namedWindow("Task 3 Left", CV_WINDOW_AUTOSIZE);
  namedWindow("Task 3 Right", CV_WINDOW_AUTOSIZE);
  moveWindow("Task 3 Left", 50, 50);
  moveWindow("Task 3 Right", 700, 50);

  path = "../images/stereo/stereoL20.bmp";
  image_left = imread( path, 1 );
  path = "../images/stereo/stereoR20.bmp";
  image_right = imread( path, 1 );

  // undistort the selected images
  Mat image_left_mod, image_right_mod;
  undistort(image_left, image_left_mod, intrinsic_left, distortion_left);
  undistort(image_right, image_right_mod, intrinsic_right, distortion_right);

  // selected points on left image:
  // center of clock              240, 62
  // top left urban challenge     120, 293
  // bottom right urban challenge 197, 342

  // selected points of right image:
  // top right chessboard         444, 273
  // middle chessboard            355, 372
  // bottom left chessboard       239, 426

  Point l1(240, 62);
  Point l2(120, 293);
  Point l3(197, 342);
  Point r1(444, 273);
  Point r2(355, 372);
  Point r3(239, 426);
  int radius = 10;
  Scalar color(0,255,255);
  int thickness = 2;

  circle(image_left_mod, l1, radius, color, thickness, 8);
  circle(image_left_mod, l2, radius, color, thickness, 8);
  circle(image_left_mod, l3, radius, color, thickness, 8);
  circle(image_right_mod, r1, radius, color, thickness, 8);
  circle(image_right_mod, r2, radius, color, thickness, 8);
  circle(image_right_mod, r3, radius, color, thickness, 8);

  std::vector<Point> l_points;
  l_points.push_back(l1);
  l_points.push_back(l2);
  l_points.push_back(l3);
  std::vector<Point> r_points;
  r_points.push_back(r1);
  r_points.push_back(r2);
  r_points.push_back(r3);

  // calculate epilines
  std::vector<Point3f> l_lines, r_lines;
  computeCorrespondEpilines(l_points, 1, F, r_lines);
  computeCorrespondEpilines(r_points, 2, F, l_lines);

  // draw epilines
  drawEpiLines(image_right_mod, r_lines);
  drawEpiLines(image_left_mod, l_lines);

  // display images with points and lines
  imshow("Task 3 Left", image_left_mod);
  imshow("Task 3 Right", image_right_mod);


////////    ///     //////  //    //    //
   //      // //   //    // //   //     //    //
   //     //   //  //       //  //      //    //
   //    //     //  //////  /////       //    //
   //    /////////       // //  //      /////////
   //    //     // //    // //   //           //
   //    //     //  //////  //    //          //


  // rectification
  namedWindow("Task 4 Left Original", CV_WINDOW_AUTOSIZE);
  namedWindow("Task 4 Right Original", CV_WINDOW_AUTOSIZE);
  moveWindow("Task 4 Left Original", 50, 500);
  moveWindow("Task 4 Right Original", 700, 500);
  imshow("Task 4 Left Original", image_left);
  imshow("Task 4 Right Original", image_right);

  namedWindow("Task 4 Left Rectified", CV_WINDOW_AUTOSIZE);
  namedWindow("Task 4 Right Rectified", CV_WINDOW_AUTOSIZE);
  moveWindow("Task 4 Left Rectified", 50, 500);
  moveWindow("Task 4 Right Rectified", 700, 500);

  namedWindow("Task 4 Left Difference", CV_WINDOW_AUTOSIZE);
  namedWindow("Task 4 Right Difference", CV_WINDOW_AUTOSIZE);
  moveWindow("Task 4 Left Difference", 50, 500);
  moveWindow("Task 4 Right Difference", 700, 500);

  Mat R1, R2, P1, P2, Q;
  stereoRectify(  intrinsic_left, distortion_left,
                  intrinsic_right, distortion_right,
                  image_left.size(),  R,  T,
                  R1, R2, P1, P2, Q  );

  // write the calibration data to "calibration_stereo.xml"
  FileStorage r_fsw("rectification.xml", FileStorage::WRITE);
  r_fsw << "R1" << R1 << "R2" << R2 << "P1" << P1 << "P2" << P2;
  r_fsw.release();

  // undistort left and right images
  Mat mapx1, mapx2, mapy1, mapy2;
  initUndistortRectifyMap(  intrinsic_left, distortion_left,
                            R1, P1, image_left.size(),
                            CV_16SC2, mapx1, mapy1  );
  initUndistortRectifyMap(  intrinsic_right, distortion_right,
                            R2, P2, image_right.size(),
                            CV_16SC2, mapx2, mapy2  );

  // remap images using calculating mapx, mapy
  remap(image_left, image_left_mod, mapx1, mapy1, INTER_LINEAR, BORDER_CONSTANT, 0);
  remap(image_right, image_right_mod, mapx2, mapy2, INTER_LINEAR, BORDER_CONSTANT, 0);

  // get the absolute difference between rectified images
  Mat diff_left, diff_right;
  absdiff(image_left_mod, image_left, diff_left);
  absdiff(image_right_mod, image_right, diff_right);

  // draw some horizontal lines on both images
  line(image_left_mod, Point(0, 75), Point(image_left_mod.cols, 75), Scalar(255, 0, 0), 1);
  line(image_left_mod, Point(0, 300), Point(image_left_mod.cols, 300), Scalar(255, 0, 0), 1);
  line(image_left_mod, Point(0, 345), Point(image_left_mod.cols, 345), Scalar(255, 0, 0), 1);
  line(image_right_mod, Point(0, 75), Point(image_right_mod.cols, 75), Scalar(255, 0, 0), 1);
  line(image_right_mod, Point(0, 300), Point(image_right_mod.cols, 300), Scalar(255, 0, 0), 1);
  line(image_right_mod, Point(0, 345), Point(image_right_mod.cols, 345), Scalar(255, 0, 0), 1);

  // show images
  imshow("Task 4 Left Difference", diff_left);
  imshow("Task 4 Right Difference", diff_right);
  imshow("Task 4 Left Rectified", image_left_mod);
  imshow("Task 4 Right Rectified", image_right_mod);


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


  printf("finished \n");
  return 0;
}
