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

  std::string           path, x_str, y_str, z_str;
  std::vector<Point2f>  centers_left,    centers_right,   // point centers
                        centers4_left,   centers4_right,  // centers of main 4
                        centers4u_left,  centers4u_right; // undistorted
  std::vector<Point3f>  centers3d_left,  centers3d_right; // 3d version

  Point2f               place;
  Size                  patternsize(h,v);
  Mat                   image_left,      image_right,
                        image_left_gray, image_right_gray,
                        intrinsic_left,  intrinsic_right,
                        distortion_left, distortion_right,
                        R,  T,  E,  F,
                        R1, R2, P1, P2, Q;

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
  // load existing rectification parameters
  FileStorage fsr4("rectification_final.xml", FileStorage::READ);
  fsr4["R1"] >> R1;
  fsr4["R2"] >> R2;
  fsr4["P1"] >> P1;
  fsr4["P2"] >> P2;
  fsr4["Q"]  >> Q;
  fsr4.release();

  // undistort the 4 corner points of left and right
  undistortPoints(centers4_left,   centers4u_left,
                  intrinsic_left,  distortion_left,
                  R1,              P1);
  undistortPoints(centers4_right,  centers4u_right,
                  intrinsic_right, distortion_right,
                  R2,              P2);

  // populate vector of Point3f by cycling through each point
  for (int i=0; i < centers4u_left.size() ; i++)
  {
    centers3d_left.push_back( Point3f(centers4u_left[i].x,  centers4u_left[i].y,  centers4u_left[i].x-centers4u_right[i].x));
    centers3d_right.push_back(Point3f(centers4u_right[i].x, centers4u_right[i].y, centers4u_left[i].x-centers4u_right[i].x));
  }

  // transform the points to calculate 3D information of 4 points
  perspectiveTransform(centers3d_left,  centers3d_left, Q);
  perspectiveTransform(centers3d_right, centers3d_right, Q);
  std::cout << centers3d_left << std::endl;
  std::cout << centers3d_right << std::endl;

  // display 3D information
  for (int i=0; i < centers4u_left.size() ; i++)
  {
    // get text format of this iteration's x, y, z for left image
    x_str = "x="+std::to_string(centers3d_left[i].x).substr(0,5);
    y_str = "y="+std::to_string(centers3d_left[i].y).substr(0,5);
    z_str = "z="+std::to_string(centers3d_left[i].z).substr(0,5);
    place.x = centers4_left[i].x + 15;
    place.y = centers4_left[i].y;
    putText(image_left, x_str, place, FONT_HERSHEY_SIMPLEX, 0.6, color1, 2);
    place.y = place.y + 20;
    putText(image_left, y_str, place, FONT_HERSHEY_SIMPLEX, 0.6, color1, 2);
    place.y = place.y + 20;
    putText(image_left, z_str, place, FONT_HERSHEY_SIMPLEX, 0.6, color1, 2);

    // get text format of this iteration's x, y, z for right image
    x_str = "x="+std::to_string(centers3d_right[i].x).substr(0,5);
    y_str = "y="+std::to_string(centers3d_right[i].y).substr(0,5);
    z_str = "z="+std::to_string(centers3d_right[i].z).substr(0,5);
    place.x = centers4_right[i].x + 15;
    place.y = centers4_right[i].y;
    putText(image_right, x_str, place, FONT_HERSHEY_SIMPLEX, 0.6, color2, 2);
    place.y = place.y + 20;
    putText(image_right, y_str, place, FONT_HERSHEY_SIMPLEX, 0.6, color2, 2);
    place.y = place.y + 20;
    putText(image_right, z_str, place, FONT_HERSHEY_SIMPLEX, 0.6, color2, 2);

  }


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


////////    ///     //////  //    //     ///////
   //      // //   //    // //   //     //     //
   //     //   //  //       //  //             //
   //    //     //  //////  /////        ///////
   //    /////////       // //  //      //
   //    //     // //    // //   //     //
   //    //     //  //////  //    //    /////////


  // this task will import the stereo pairs
  // at each iteration, the ball is found in the roi, then roi is updated

  Mat key_image_left, key_image_right,
      image_left_mod, image_right_mod;

  namedWindow("Task 2 Left", CV_WINDOW_AUTOSIZE);
  namedWindow("Task 2 Right", CV_WINDOW_AUTOSIZE);
  moveWindow("Task 2 Left", 50, 500);
  moveWindow("Task 2 Right", 700, 500);

  // parameters for the open operation
  int morph_size = 2;
  Mat element = getStructuringElement( MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );

  // initialize params and modify properties for the blob detector
  SimpleBlobDetector::Params params;
  params.thresholdStep = 10;
  params.minThreshold = 50;
  params.maxThreshold = 220;
  params.minRepeatability = 2;
  params.minDistBetweenBlobs = 10;
  params.filterByColor = false; // true
  params.blobColor = 0;
  params.filterByArea = true;
  params.minArea = 25;
  params.maxArea = 5000;
  params.filterByCircularity = false;
  params.minCircularity = 0.8f;
  params.maxCircularity = std::numeric_limits<float>::max();
  params.filterByInertia = false; // true
  params.minInertiaRatio = 0.1f;
  params.maxInertiaRatio = std::numeric_limits<float>::max();
  params.filterByConvexity = false; // true
  params.minConvexity = 0.95f;
  params.maxConvexity = std::numeric_limits<float>::max();

  // initialize keypoints vector
  std::vector<KeyPoint> keypoints_left;
  std::vector<KeyPoint> keypoints_right;

  // create the detector(pointer) using the params
  Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

  // define initial centers of ROI
  Point2f roi_left  = Point2f(357, 115);
  Point2f roi_right = Point2f(285, 122);
  // define height and width of ROI
  float roi_x = 50; // roi width  / 2
  float roi_y = 50; // roi height / 2

  // loop through images, detecting the ball
  int n_img = 1;
  while(1)
  {
    // get left and right images
    path = "../images/trajectory/Ball_testL"+std::to_string(n_img)+".bmp";
    image_left = imread( path, 1 );
    path = "../images/trajectory/Ball_testR"+std::to_string(n_img)+".bmp";
    image_right = imread( path, 1 );

    // test image validity
    if ( !image_left.data || !image_right.data )
    {
      break;
    }

    // if image is the first image in the set, keep it for difference images
    if (n_img == 1)
    {
      key_image_left = image_left.clone();
      key_image_right = image_right.clone();
    }

    // extract the roi
    /*
    image_left = image_left_orig(Rect(roi_left.x-roi_x, roi_left.y-roi_y, roi_x*2, roi_y*2));
    image_right =
    key_image_left =
    key_image_right =
    */

    // take absolute difference between current images and keyframes
    absdiff(image_left,  key_image_left,  image_left_mod);
    absdiff(image_right, key_image_right, image_right_mod);
    // convert to grayscale
    cvtColor(image_left_mod,  image_left_mod,  CV_BGR2GRAY);
    cvtColor(image_right_mod, image_right_mod, CV_BGR2GRAY);
    // threshold the grayscale difference image
    threshold(image_left_mod,  image_left_mod,  5, 255, THRESH_BINARY);
    threshold(image_right_mod, image_right_mod, 5, 255, THRESH_BINARY);
    // perform open operation
    morphologyEx(image_left_mod,  image_left_mod,  MORPH_OPEN, element );
    morphologyEx(image_right_mod, image_right_mod, MORPH_OPEN, element );
    // use blob detector to populate the vector of keypoints
    detector->detect( image_left_mod,  keypoints_left );
    detector->detect( image_right_mod, keypoints_right );
    // draw points on main image
    drawKeypoints(image_left,  keypoints_left,  image_left,  Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    drawKeypoints(image_right, keypoints_right, image_right, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);




    // update ROI
    float x_sum = 0.0;
    float y_sum = 0.0;
    int i;
    // average the locations of the keypoints
    for (i=0; i < keypoints_left.size() ; i++)
    {
      x_sum = x_sum + keypoints_left[i].pt.x;
      y_sum = y_sum + keypoints_left[i].pt.y;
    }
    // if keypoints were found, update roi center
    if (x_sum > 0 && y_sum > 0)
    {
      roi_left.x = x_sum/i;
      roi_left.y = y_sum/i;
    }


    // draw updated ROI
    //circle(image_left, roi_left, 10, color1, 2);
    // define upper left and lower right corners of ROI
    Point2f ul = Point2f(roi_left.x-roi_x, roi_left.y-roi_y);
    Point2f lr = Point2f(roi_left.x+roi_x, roi_left.y+roi_y);
    //circle(image_left, ul, 10, color2, 2);
    //circle(image_left, lr, 10, color2, 2);
    rectangle(image_left, ul, lr, color1, 2);




    imshow("Task 2 Left", image_left);
    imshow("Task 2 Right", image_right);

    // wait for a new key input
    key = waitKey();
    if (key == 110)
    {
      // the 'n' (next) key was pressed, continue to next image
    }
    else if (key == 27)
    {
      // the 'esc' key was pressed, end application
      std::cout << "terminating" << std::endl;
      break;
    }

    n_img++;

  }














  printf("finished \n");
  return 0;
}
