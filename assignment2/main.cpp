//#include <stdio.h> // from OpenCV introduction
#include <opencv2/opencv.hpp>
#include <string> // needed for setting std::strings and using to_string
#include <fstream>

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
    //std::cout << "next task" << std::endl;
  }
  else if (key == 27)
  {
    // the 'esc' key was pressed, end application
    std::cout << "terminating" << std::endl;
    return -1;
  }

  // ------------------------------ TASK 2 ------------------------------

  bool recalibrate = false;
  if (recalibrate)
  {
    // initialize vector of vectors of 2-D floating points
    std::vector<std::vector<Point2f>> img_points;
    std::vector<std::vector<Point3f>> obj_points;
    std::vector<Point3f> obj_set;
    Point3f p_cur;

    // obj_set is set of world frame checker points (0,0,0), (1,0,0) ... (n,m,0)
    // each image will have a corresponding one, but they are all identical
    // based on the way the calibrate camera defines the world frames

    // create a vector of 3d points corresponding
    for (int y=0; y<v; y++)
    {
      for (int x=0; x<h; x++)
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


    Mat intrinsic = Mat(3, 3, CV_64FC1);
    Mat distortion = Mat(5, 1, CV_64FC1);
    std::vector<Mat> rvecs;
    std::vector<Mat> tvecs;
    // now use the overall vectors to get the camera calibration
    std::cout << "calculating..." << std::endl;
    calibrateCamera(obj_points, img_points, image.size(), intrinsic, distortion, rvecs, tvecs);

    std::cout << intrinsic << std::endl;
    std::cout << distortion << std::endl;

    // write the calibration data to "calibration.xml"
    FileStorage fsw("calibration.xml", FileStorage::WRITE);
    fsw << "intrinsic" << intrinsic << "distortion" << distortion;
    fsw.release();


    // wait for a new key input
    key = waitKey();
    if (key == 110)
    {
      // the 'n' (next) key was pressed, increment image
      std::cout << "moving on to task 3" << std::endl;
    }
    else if (key == 27)
    {
      // the 'esc' key was pressed, end application
      std::cout << "terminating" << std::endl;
      return -1;
    }
  }

  // ------------------------------ TASK 3 ------------------------------
  // create display window
  namedWindow("Task 3a", CV_WINDOW_AUTOSIZE);
  namedWindow("Task 3b", CV_WINDOW_AUTOSIZE);

  // read the calibration data
  FileStorage fsr("calibration_final.xml", FileStorage::READ);
  Mat intrinsic2, distortion2;
  fsr["intrinsic"] >> intrinsic2;
  fsr["distortion"] >> distortion2;
  fsr.release();


  // create an array with the image names to undistort
  std::string names[3] = {"Close.jpg", "Far.jpg", "Turn.jpg"};


  for (int j = 0; j<3; j++)
  {
    // extract current image to be undistorted
    path = "../images/" + names[j];
    image = imread(path, 1);
    imshow("Task 3a", image);

    // run function
    Mat image2;
    undistort(image, image2, intrinsic2, distortion2);

    // generate difference image
    Mat image3;
    absdiff(image, image2, image3);

    // display undistorted image
    imshow("Task 3b", image3);



    // wait for a new key input
    key = waitKey();
    if (key == 110)
    {
      // the 'n' (next) key was pressed, increment image
      //std::cout << "moving on to task 3" << std::endl;
    }
    else if (key == 27)
    {
      // the 'esc' key was pressed, end application
      std::cout << "terminating" << std::endl;
      return -1;
    }


  }


  // ------------------------------ TASK 4 ------------------------------
  // load the known "object with corners"
  path = "../images/Close.jpg";
  image = imread(path, 1);

  std::vector<Point2f> img_points2;
  std::vector<Point3f> obj_points2;
  Point2f p_cur2;
  Point3f p_cur3;

  // read in these points from txt file
  std::ifstream file123("../data_points.txt");

  // cycle through txt for 2d points
  for (int k=0; k<20; k++)
  {
    file123 >> p_cur2.x;
    file123 >> p_cur2.y;
    img_points2.push_back(p_cur2);
  }

  // cycle through txt for 3d points
  for (int k=0; k<20; k++)
  {
    file123 >> p_cur3.x;
    file123 >> p_cur3.y;
    file123 >> p_cur3.z;
    obj_points2.push_back(p_cur3);
  }

  // use 2d and 3d points with the solvepnp function
  Mat rvec, rmat, tvec;
  bool solved = solvePnP(obj_points2, img_points2, intrinsic2, distortion2, rvec, tvec);

  // convert rotation vector to rotation matrix
  Rodrigues(rvec, rmat);

  //std::cout << rmat << std::endl;
  //std::cout << tvec << std::endl;

  // write the pose data to "pose.yml"
  FileStorage fsw2("pose.yml", FileStorage::WRITE);
  fsw2 << "rotation" << rmat << "translation" << tvec;
  fsw2.release();

  // wait for a new key input
  key = waitKey();
  if (key == 110)
  {
    // the 'n' (next) key was pressed, increment image
    //std::cout << "moving on to task 3" << std::endl;
  }
  else if (key == 27)
  {
    // the 'esc' key was pressed, end application
    std::cout << "terminating" << std::endl;
    return -1;
  }

  // ------------------------------ TASK 5 ------------------------------

  namedWindow("Task 5", CV_WINDOW_AUTOSIZE);

  // reset parameters for my chessboard
  h = 9; // horizontal internal points
  v = 7; // vertical internal points
  Size patternsize2(h,v);

  RNG rng;

  recalibrate = false;
  if (recalibrate)
  {
    // initialize vector of vectors of 2-D floating points
    std::vector<std::vector<Point2f>> img_points_own;
    std::vector<std::vector<Point3f>> obj_points_own;
    std::vector<Point3f> obj_set_own;
    Point3f p_cur_own;
    centers.clear();

    // obj_set is set of world frame checker points (0,0,0), (1,0,0) ... (n,m,0)
    // each image will have a corresponding one, but they are all identical
    // based on the way the calibrate camera defines the world frames

    // create a vector of 3d points corresponding
    for (int y=0; y<v; y++)
    {
      for (int x=0; x<h; x++)
      {
        // generate the current point
        p_cur_own.x = x;
        p_cur_own.y = y;
        p_cur_own.z = 0.0f;

        obj_set_own.push_back(p_cur_own);
      }
    }


    VideoCapture video(0);    // get a camera object

    // cycle through live video frames until enough images are found
    int i=0;
    while(i<40)
    {
      found = false;
      while(!found) // cycle video until chessboard is in view
      {
        // grab a frame
        video >> image;

        // convert image format to grayscale
        cvtColor(image, image_gray, CV_BGR2GRAY);

        // find the chessboard corners
        found = findChessboardCorners(image_gray, patternsize2, centers);

        // display image
        imshow("Task 5", image);

        key = waitKey(10);
      }


      // refine corner locations with subpixel accuracy
      cornerSubPix(image_gray, centers, Size(5,5), Size(-1,-1), TermCriteria(3,30,0.1));

      // randomly sample whether or not to accept frame
      // this is to give user time to move chessboard around
      int sel = rng.uniform(1, 10);
      if (sel==1)
      {
        // add the resulting vector of points to the overall vector
        img_points_own.push_back(centers);
        obj_points_own.push_back(obj_set_own);

        i++;
      }

      // draw the corners in color on the original image
      drawChessboardCorners(image, patternsize2, Mat(centers), found);

      // display chessboard points live
      imshow("Task 5", image);

      key = waitKey(10);

    }

    // now we have vectors of vectors for calibration

    Mat intrinsic_own = Mat(3, 3, CV_64FC1);
    Mat distortion_own = Mat(5, 1, CV_64FC1);
    std::vector<Mat> rvecs_own;
    std::vector<Mat> tvecs_own;
    // now use the overall vectors to get the camera calibration
    std::cout << "calculating..." << std::endl;
    calibrateCamera(obj_points_own, img_points_own, image.size(), intrinsic_own, distortion_own, rvecs_own, tvecs_own);

    std::cout << intrinsic_own << std::endl;
    std::cout << distortion_own << std::endl;

    // write the calibration data to "calibration_own.xml"
    FileStorage fsw("calibration_own.xml", FileStorage::WRITE);
    fsw << "intrinsic" << intrinsic_own << "distortion" << distortion_own;
    fsw.release();


    // wait for a new key input
    key = waitKey();
    if (key == 110)
    {
      // the 'n' (next) key was pressed, increment image
      //std::cout << "moving on to task 3" << std::endl;
    }
    else if (key == 27)
    {
      // the 'esc' key was pressed, end application
      std::cout << "terminating" << std::endl;
      return -1;
    }
  }


  // ------------------------------ TASK 6 ------------------------------
  // create display window
  namedWindow("Task 6a", CV_WINDOW_AUTOSIZE);
  namedWindow("Task 6b", CV_WINDOW_AUTOSIZE);

  // read the calibration data
  FileStorage fsr2("calibration_own_final.xml", FileStorage::READ);
  Mat intrinsic_own2, distortion_own2;
  fsr2["intrinsic"] >> intrinsic_own2;
  fsr2["distortion"] >> distortion_own2;
  fsr2.release();

  // extract current image to be undistorted
  path = "../images/example.jpg";
  image = imread(path, 1);
  imshow("Task 6a", image);

  // run function
  Mat image2;
  undistort(image, image2, intrinsic_own2, distortion_own2);

  // generate difference image
  Mat image3;
  absdiff(image, image2, image3);

  // display undistorted image
  imshow("Task 6b", image2);



  // wait for a new key input
  key = waitKey();
  if (key == 110)
  {
    // the 'n' (next) key was pressed, increment image
    //std::cout << "moving on to task 3" << std::endl;
  }
  else if (key == 27)
  {
    // the 'esc' key was pressed, end application
    std::cout << "terminating" << std::endl;
    return -1;
  }






return 0;
}
