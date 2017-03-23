#include <opencv2/opencv.hpp>
#include <string> // needed for setting std::strings and using to_string
#include <fstream>

using namespace cv;

void drawPoints(Mat img, std::vector<Point2f>& points){
  for (int i=0; i < points.size(); i++){
    circle(img, points[i], 2, Scalar(0, 255, 0), 2);
  }
}

void drawRedLines(Mat img, std::vector<Point2f>& points_a,
                           std::vector<Point2f>& points_b,
                           std::vector<uchar>&   status_vec){
  uint status; // for conversion of uchar to uint
  for (int i=0; i < status_vec.size(); i++){
    status = status_vec[i];
    if (status == 1){
      // plot stuffs here
      arrowedLine(img, points_a[i], points_b[i], Scalar(0, 0, 255), 2);
    }
  }
}

void drawRedLinesSimple(Mat img, std::vector<Point2f>& points_a,
                                 std::vector<Point2f>& points_b){
  for (int i=0; i < points_a.size(); i++){
    // plot stuffs here
    arrowedLine(img, points_a[i], points_b[i], Scalar(0, 0, 255), 2);
  }
}


int main(int argc, char** argv )
{

   // initialize variables and windows
   String path;

   std::vector<String> set; // for generating path of each
   //set.push_back("Optical Flow");
   set.push_back("Parallel Cube");
   set.push_back("Parallel Real");
   set.push_back("Turned Cube");
   set.push_back("Turned Real");

   std::vector<String> prefix; // for file names for path
   //prefix.push_back("O");
   prefix.push_back("ParallelCube");
   prefix.push_back("ParallelReal");
   prefix.push_back("TurnCube");
   prefix.push_back("TurnReal");

   std::vector<int> nn; // for ending inner loop
   //nn.push_back(17);
   nn.push_back(18);
   nn.push_back(18);
   nn.push_back(18);
   nn.push_back(19);

   std::vector<Point2f> features_a, features_b;

   Mat image_a, image_a_mod,
       image_b, image_b_mod;


  // task 1 (copied from last assignment's task 3)
  namedWindow("Task 1 A", CV_WINDOW_AUTOSIZE);
  namedWindow("Task 1 B", CV_WINDOW_AUTOSIZE);
  moveWindow("Task 1 A", 50, 50);
  moveWindow("Task 1 B", 700, 50);

  std::vector<Point2f> features_keep_a, features_keep_a_temp, features_keep_b_temp;
  Mat img_10;
  bool task1 = true;
  int gap = 1;
  if (task1)
  {

    // cycle through image set types
    for (int i=0; i<4; i++)
    {
      // cycle through the images of each set
      for (int j=10; j<= 15; j=j+gap)
      {
        // get images
        path = "../../assignment5/images/" + set[i] + "/" + prefix[i] + std::to_string(j) + ".jpg";
        image_a = imread( path, 1 );
        path = "../../assignment5/images/" + set[i] + "/" + prefix[i] + std::to_string(j+gap) + ".jpg";
        image_b = imread( path, 1 );

        // test image validity
        if ( !image_a.data || !image_b.data ) {
          //printf("No image data \n");
          break;
        } else {

          // perform operations here between a and b: convert to gray
          cvtColor(image_a, image_a_mod, CV_BGR2GRAY);
          cvtColor(image_b, image_b_mod, CV_BGR2GRAY);


          // goodFeaturesToTrack for first image in sequence
          if (j==10){
            features_a.clear();
            features_b.clear();
            int max_points = 1000;
            double quality = 0.01;
            double min_dist = 10;
            Mat mask;
            int blockSize = 3;
            bool useHarrisDetector = false;
            double k = 0.04;
            goodFeaturesToTrack(image_a_mod, features_a, max_points, quality, min_dist, mask, blockSize, useHarrisDetector, k);
            features_keep_a = features_a;
          } else {
            features_a.clear();
            features_a = features_b;
            features_b.clear();
          }

          // template matching
          int dim_win = 81;
          int dim_tem = 31;
          for (int k=0; k<features_a.size(); k++)
          {
            // create a window around features_a[k], which is Point2f
            int x1 = features_a[k].x - dim_win/2;
            if (x1 < 0)
            {
              x1 = 0;
            }
            if ((x1+dim_win)>640)
            {
              x1 = 640 - dim_win;
            }
            int y1 = features_a[k].y - dim_win/2;
            if (y1 < 0)
            {
              y1 = 0;
            }
            if ((y1+dim_win)>480)
            {
              y1 = 480 - dim_win;
            }
            // window established in next image
            Mat win = image_b_mod(Rect(x1, y1, dim_win, dim_win));

            // create a template around features_a[k], which is Point2f
            int x2 = features_a[k].x - dim_tem/2;
            if (x2 < 0)
            {
              x2 = 0;
              features_a[k].x = x2 + dim_tem/2;
            }
            if ((x2+dim_tem)>640)
            {
              x2 = 640 - dim_tem;
              features_a[k].x = x2 + dim_tem/2;
            }
            int y2 = features_a[k].y - dim_tem/2;
            if (y2 < 0)
            {
              y2 = 0;
              features_a[k].y = y2 + dim_tem/2;
            }
            if ((y2+dim_tem)>480)
            {
              y2 = 480 - dim_tem;
              features_a[k].y = y2 + dim_tem/2;
            }
            // window established in next image
            Mat tem = image_a_mod(Rect(x2, y2, dim_tem, dim_tem));

            // match the current template of image 1 to window of image 2
            Mat output;
            matchTemplate(win, tem, output, TM_CCORR_NORMED);

            // normalize the intensity output
            normalize(output, output, 0, 1, NORM_MINMAX,  -1, Mat());

            // locate the position of highest correlation
            Point max_point;
            minMaxLoc(output, 0, 0, 0, &max_point, Mat());

            // represent the max point in original image coordinates
            max_point.x = max_point.x + dim_tem/2 + x1;
            max_point.y = max_point.y + dim_tem/2 + y1;

            // populate the vector of features_b
            features_b.push_back(max_point);
          }

          // find the fundamental matrix, get list of outliers
          std::vector<uchar> status_vec;
          Mat F = findFundamentalMat(features_a, features_b, FM_RANSAC, 3.0, 0.99, status_vec);

          // convert color back to BGR
          cvtColor(image_a_mod, image_a_mod, CV_GRAY2BGR);
          cvtColor(image_b_mod, image_b_mod, CV_GRAY2BGR);


          if (j==10)
          {
            img_10 = image_a_mod.clone();
          }

          /*
          img_10                  the original image of the sequence
          features_keep_a         features that are kept for final plotting
          features_keep_a_temp    used for eliminating features using status_vec
          features_keep_b_temp    used for eliminating features using status_vec
          */

          // remove outlier features
          features_keep_a_temp.clear();
          features_keep_b_temp.clear();
          uint status; // for conversion of uchar to uint
          for (int k = 0; k < features_b.size(); k++)
          {
            status = status_vec[k];
            if (status == 1)
            {
              // current feature is an inlier, keep it
              features_keep_a_temp.push_back(features_keep_a[k]);
              features_keep_b_temp.push_back(features_b[k]);
            }
          }

          features_keep_a.clear();
          features_keep_a = features_keep_a_temp;

          features_b.clear();
          features_b = features_keep_b_temp;

          // only draw and plot using the original and final images
          if (false) //if (j==15)
          {

            // draw feature points and red lines
            drawPoints(img_10, features_keep_a);
            drawPoints(image_b_mod, features_b);
            drawRedLinesSimple(img_10, features_keep_a, features_b);

            // display images a and b
            imshow("Task 1 A", img_10);
            imshow("Task 1 B", image_b_mod);
          }

        }

        // wait for a new key input after operations and display are complete
        // if end of the sequence is reached



      } // end for loop


////////    ///     //////  //    //       //
   //      // //   //    // //   //      ////
   //     //   //  //       //  //         //
   //    //     //  //////  /////          //
   //    /////////       // //  //         //
   //    //     // //    // //   //        //
   //    //     //  //////  //    //     //////


      // FINISHED CURRENT IMAGE TYPE
      // features_keep_a  are the original GFTT from the first image
      // features_b       are the surviving corresponding feature in last image
      // status_vec       is the vector specifying which features are good
      // img_10           the Mat that is the original image
      // image_b_mod      the Mat that is the final image

      // find the fundamental matrix between first and last with outliers
      std::vector<uchar> status_vec;
      Mat F = findFundamentalMat(features_keep_a, features_b, FM_8POINT, 3.0, 0.99);

      /*
      // remove outlier features (only for FM_RANSAC method above)
      features_keep_a_temp.clear();
      features_keep_b_temp.clear();
      uint status; // for conversion of uchar to uint
      for (int kk = 0; kk < features_b.size(); kk++)
      {
        status = status_vec[kk];
        if (status == 1)
        {
          // current feature is an inlier, keep it
          features_keep_a_temp.push_back(features_keep_a[kk]);
          features_keep_b_temp.push_back(features_b[kk]);
        }
      }


      features_keep_a.clear();
      features_keep_a = features_keep_a_temp;

      features_b.clear();
      features_b = features_keep_b_temp;
      */


      // find the homography matricies
      Mat H1, H2;
      bool out = stereoRectifyUncalibrated(features_keep_a, features_b, F,
                                            img_10.size(), H1, H2);

      // undistort the images
      // use estimated camera matrix and distortion coefficients
      FileStorage fsr1("calibration_guess_final.xml", FileStorage::READ);
      Mat intrinsic, distortion, R1, R2;
      fsr1["intrinsic"] >> intrinsic;
      fsr1["distortion"] >> distortion;
      fsr1.release();


      R1 = intrinsic.inv() * H1 * intrinsic;
      R2 = intrinsic.inv() * H2 * intrinsic;

      Mat mapx1, mapx2, mapy1, mapy2;
      initUndistortRectifyMap( intrinsic, distortion, R1, intrinsic,
                                img_10.size(), CV_16SC2, mapx1, mapy1 );
      initUndistortRectifyMap( intrinsic, distortion, R2, intrinsic,
                                img_10.size(), CV_16SC2, mapx2, mapy2 );

      // remap
      remap(img_10, img_10,           mapx1, mapy1, INTER_LINEAR, BORDER_CONSTANT, 0);
      remap(image_b_mod, image_b_mod, mapx2, mapy2, INTER_LINEAR, BORDER_CONSTANT, 0);


      // draw horizontal lines on images
      for (int kk = 1; kk<=4; kk++)
      {
        line(img_10,      Point2f(0,96*kk), Point2f(640,96*kk), Scalar(255,0,0));
        line(image_b_mod, Point2f(0,96*kk), Point2f(640,96*kk), Scalar(255,0,0));
      }



      // display the images
      imshow("Task 1 A", img_10);
      imshow("Task 1 B", image_b_mod);


      ////////    ///     //////  //    //     ///////
         //      // //   //    // //   //     //     //
         //     //   //  //       //  //             //
         //    //     //  //////  /////        ///////
         //    /////////       // //  //      //
         //    //     // //    // //   //     //
         //    //     //  //////  //    //    /////////


      // task 2
      // use given sfm camera matrix and distortion coefficients
      FileStorage fsr2("sfm_camera_parameters_final.xml", FileStorage::READ);
      fsr2["intrinsic"] >> intrinsic;
      fsr2["distortion"] >> distortion;
      fsr2.release();


      undistortPoints(features_keep_a, features_b, intrinsic, distortion);



      int key = waitKey();
      if (key == 110) {
        // the 'n' (next) key was pressed
      } else if (key == 27) {
        // the 'esc' key was pressed, end application
        std::cout << "terminating" << std::endl;
        return -1;
      }


    } // end for loop
  } // end if true statement









  printf("finished \n");
  return 0;
}
