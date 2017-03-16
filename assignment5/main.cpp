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
  String path;

  std::vector<String> set; // for generating path of each
  set.push_back("Optical Flow");
  set.push_back("Parallel Cube");
  set.push_back("Parallel Real");
  set.push_back("Turned Cube");
  set.push_back("Turned Real");

  std::vector<String> prefix; // for file names for path
  prefix.push_back("O");
  prefix.push_back("ParallelCube");
  prefix.push_back("ParallelReal");
  prefix.push_back("TurnCube");
  prefix.push_back("TurnReal");

  std::vector<int> nn; // for ending inner loop
  nn.push_back(17);
  nn.push_back(18);
  nn.push_back(18);
  nn.push_back(18);
  nn.push_back(19);

  std::vector<Point2f> features_a, features_b;

  Mat image_a, image_a_mod,
      image_b, image_b_mod;
  String file = "task1.avi";

  namedWindow("Task 1 A", CV_WINDOW_AUTOSIZE);
  namedWindow("Task 1 B", CV_WINDOW_AUTOSIZE);
  moveWindow("Task 1 A", 50, 50);
  moveWindow("Task 1 B", 700, 50);

  VideoWriter VOut;         // Create a video write object.
  // initialize video write object
  VOut.open(file, CV_FOURCC('M', 'P', 'E', 'G') , 30, Size(640, 480), 1);
  // alternative syntax if missing codec
  //VOut.open("VideoOut.avi", -1 , 30, Size(640, 480), 1);

  bool task1 = false;
  if (task1)
  {
    for (int gap=1; gap<=3; gap++)
    {
      // cycle through image set types (for taks 1, "i<1")
      for (int i=0; i<1; i++){
        // cycle through the images of each set
        for (int j=1; j<= nn[i]; j=j+gap){

          // get images
          path = "../images/" + set[i] + "/" + prefix[i] + std::to_string(j) + ".jpg";
          image_a = imread( path, 1 );
          path = "../images/" + set[i] + "/" + prefix[i] + std::to_string(j+gap) + ".jpg";
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
            if (j==1){
              int max_points = 500;
              double quality = 0.01;
              double min_dist = 10;
              Mat mask;
              int blockSize = 3;
              bool useHarrisDetector = false;
              double k = 0.04;
              goodFeaturesToTrack(image_a_mod, features_a, max_points, quality, min_dist, mask, blockSize, useHarrisDetector, k);
            } else {
              features_a.clear();
              features_a = features_b;
              features_b.clear();
            }

            // LK pyramid method to get features_b, create pyramids
            std::vector<Mat> pyramid_a, pyramid_b;
            Size pyr_win = Size(21,21);
            int pyr_max_lvl = 3;
            buildOpticalFlowPyramid(image_a_mod, pyramid_a, pyr_win, pyr_max_lvl);
            buildOpticalFlowPyramid(image_b_mod, pyramid_b, pyr_win, pyr_max_lvl);

            // perform optical flow
            std::vector<uchar> status;
            std::vector<float> err;
            TermCriteria criteria = TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, 0.01);
            int flags = 0;
            double minEigThreshold = 1e-4;
            calcOpticalFlowPyrLK	(	pyramid_a,    pyramid_b,
                                    features_a,   features_b,
                                    status,       err,
                                    pyr_win, pyr_max_lvl, criteria,
                                    flags, minEigThreshold);

            // convert color back to BGR
            cvtColor(image_a_mod, image_a_mod, CV_GRAY2BGR);
            cvtColor(image_b_mod, image_b_mod, CV_GRAY2BGR);

            // draw feature points and red lines
            drawPoints(image_a_mod, features_a);
            drawPoints(image_b_mod, features_b);
            drawRedLines(image_a_mod, features_a, features_b, status);

            // display images a and b
            imshow("Task 1 A", image_a_mod);
            imshow("Task 1 B", image_b_mod);

            // PUT INTO AVI, save and commit
            VOut << image_a_mod;            // save frame to video file
            // DO VARIOUS STRIDE VALUES

          }

          // wait for a new key input after operations and display are complete
          int key = waitKey();
          if (key == 110) {
            // the 'n' (next) key was pressed
          } else if (key == 27) {
            // the 'esc' key was pressed, end application
            std::cout << "terminating" << std::endl;
            return -1;
          }


        } // end for loop
      } // end for loop
    }
  }


////////    ///     //////  //    //     ///////
   //      // //   //    // //   //     //     //
   //     //   //  //       //  //             //
   //    //     //  //////  /////        ///////
   //    /////////       // //  //      //
   //    //     // //    // //   //     //
   //    //     //  //////  //    //    /////////


  // task 2
  namedWindow("Task 2 A", CV_WINDOW_AUTOSIZE);
  namedWindow("Task 2 B", CV_WINDOW_AUTOSIZE);
  moveWindow("Task 2 A", 50, 50);
  moveWindow("Task 2 B", 700, 50);

  VideoWriter VOut2;         // Create a video write object.
  file = "task2.avi";
  // initialize video write object
  VOut2.open(file, CV_FOURCC('M', 'P', 'E', 'G') , 30, Size(640, 480), 1);
  // alternative syntax if missing codec
  //VOut.open("VideoOut.avi", -1 , 30, Size(640, 480), 1);

  bool task2 = true;
  if (true)
  {
    for (int gap=1; gap<=3; gap++)
    {
      // cycle through image set types (for taks 1, "i<1")
      for (int i=0; i<1; i++){
        // cycle through the images of each set
        for (int j=1; j<= nn[i]; j=j+gap){

          // get images
          path = "../images/" + set[i] + "/" + prefix[i] + std::to_string(j) + ".jpg";
          image_a = imread( path, 1 );
          path = "../images/" + set[i] + "/" + prefix[i] + std::to_string(j+gap) + ".jpg";
          image_b = imread( path, 1 );

          // test image validity
          if ( !image_a.data || !image_b.data ) {
            //printf("No image data \n");
            break;
          } else {

            // perform operations here between a and b: convert to gray
            cvtColor(image_a, image_a_mod, CV_BGR2GRAY);
            cvtColor(image_b, image_b_mod, CV_BGR2GRAY);

            /*
            // artifacts from task 1 pasting:
            // goodFeaturesToTrack for first image in sequence
            if (j==1){
              int max_points = 500;
              double quality = 0.01;
              double min_dist = 10;
              Mat mask;
              int blockSize = 3;
              bool useHarrisDetector = false;
              double k = 0.04;
              goodFeaturesToTrack(image_a_mod, features_a, max_points, quality, min_dist, mask, blockSize, useHarrisDetector, k);
            } else {
              features_a.clear();
              features_a = features_b;
              features_b.clear();
            }

            // LK pyramid method to get features_b, create pyramids
            std::vector<Mat> pyramid_a, pyramid_b;
            Size pyr_win = Size(21,21);
            int pyr_max_lvl = 3;
            buildOpticalFlowPyramid(image_a_mod, pyramid_a, pyr_win, pyr_max_lvl);
            buildOpticalFlowPyramid(image_b_mod, pyramid_b, pyr_win, pyr_max_lvl);

            // perform optical flow
            std::vector<uchar> status;
            std::vector<float> err;
            TermCriteria criteria = TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, 0.01);
            int flags = 0;
            double minEigThreshold = 1e-4;
            calcOpticalFlowPyrLK	(	pyramid_a,    pyramid_b,
                                    features_a,   features_b,
                                    status,       err,
                                    pyr_win, pyr_max_lvl, criteria,
                                    flags, minEigThreshold);

            // convert color back to BGR
            cvtColor(image_a_mod, image_a_mod, CV_GRAY2BGR);
            cvtColor(image_b_mod, image_b_mod, CV_GRAY2BGR);

            // draw feature points and red lines
            drawPoints(image_a_mod, features_a);
            drawPoints(image_b_mod, features_b);
            drawRedLines(image_a_mod, features_a, features_b, status);
            */
            // display images a and b
            imshow("Task 2 A", image_a_mod);
            imshow("Task 2 B", image_b_mod);

            // PUT INTO AVI, save and commit
            VOut2 << image_a_mod;            // save frame to video file
            // DO VARIOUS STRIDE VALUES

          }

          // wait for a new key input after operations and display are complete
          int key = waitKey();
          if (key == 110) {
            // the 'n' (next) key was pressed
          } else if (key == 27) {
            // the 'esc' key was pressed, end application
            std::cout << "terminating" << std::endl;
            return -1;
          }


        } // end for loop
      } // end for loop
    }
  }











  printf("finished \n");
  return 0;
}
