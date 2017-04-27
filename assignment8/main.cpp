#include <opencv2/opencv.hpp>
#include <string> // needed for setting std::strings and using to_string
#include <fstream>
#include <iostream>

#include <Python.h>

using namespace cv;


void drawFeatures(Mat img, std::vector<Point2f>& features)
{
  for (int i=0; i < features.size(); i++)
  {
    circle(img, features[i], 2, Scalar(255, 0, 0), 2);
  }
}


void cleanFeatures(std::vector<Point2f>& features_old, std::vector<Point2f>& features_new, std::vector<uchar> features_mask)
{
  // the purpose of this method is to clean up the original and recent
  // feature vectors based on the class member "features_mask"
  std::vector<cv::Point2f> features_a, features_b, features_c;

  for (int i=0; i<features_mask.size(); i++)
  {
    if (features_mask[i] == 1)
    {
      // keep the current feature
      //features_a.push_back(features_all[0][i]);
      features_b.push_back(features_new[i]);
      features_c.push_back(features_old[i]);
    }
  }

  // update feature vectors
  //features_all[0]   = features_a;
  features_new = features_b;
  features_old = features_c;

  // clean the mask
  //features_mask.clear();
}

void cleanFeatures2(std::vector<Point2f>& features_old, std::vector<Point2f>& features_new, std::vector<uchar> features_mask, std::vector<float> err, double minEigThreshold)
{
  // the purpose of this method is to clean up the original and recent
  // feature vectors based on the class member "features_mask"
  std::vector<cv::Point2f> features_a, features_b, features_c;

  for (int i=0; i<features_mask.size(); i++)
  {
    if (features_mask[i] == 1 && err[i] > minEigThreshold)
    {
      // keep the current feature
      //features_a.push_back(features_all[0][i]);
      features_b.push_back(features_new[i]);
      features_c.push_back(features_old[i]);
    }
  }

  // update feature vectors
  //features_all[0]   = features_a;
  features_new = features_b;
  features_old = features_c;

  // clean the mask
  //features_mask.clear();
}



int main(int argc, char** argv )
{




  // determines when to stop the algorithm
  bool feed = true;
  bool live_plotting = true;
  bool intensity = false;
  bool lkflow = true;
  int ceiling = 10000;

  // whether or not to use practice images
  std::string set;
  //set = "practice";
  set = "hall";
  //set = "urban";

  // import camera parameters
  // choose path to parameters based on image set
  std::string path;
  if(set == "practice")
  {
    path = "practice/VO Practice Camera Parameters.txt";
  }
  else if (set == "hall")
  {
    path = "hall/BYU Hallway Camera Parameters.txt";
  }
  else
  {
    // no parameters supplied, use the practice ones
    path = "practice/VO Practice Camera Parameters.txt";
  }

  Mat intrinsic (Size(3,3), CV_64FC1);
  std::ifstream file_read;
  file_read.open(path);
  file_read >> intrinsic.at<double>(0,0);
  file_read >> intrinsic.at<double>(0,1);
  file_read >> intrinsic.at<double>(0,2);
  file_read >> intrinsic.at<double>(1,0);
  file_read >> intrinsic.at<double>(1,1);
  file_read >> intrinsic.at<double>(1,2);
  file_read >> intrinsic.at<double>(2,0);
  file_read >> intrinsic.at<double>(2,1);
  file_read >> intrinsic.at<double>(2,2);
  file_read.close();

  // set first frame index
  int frame_idx = 0;

  std::ofstream save;
  save.open("rt.txt");

  Mat frame;
  Mat frame_old;
  Mat R = Mat::eye(3,3, CV_64F);
  Mat T(Size(1,3), CV_64F);
  T.at<double>(0,0) = 0;
  T.at<double>(0,1) = 0;
  T.at<double>(0,2) = 0;
  Mat Ck = Mat::eye(4,4, CV_64F);



  if (live_plotting)
  {
    // initialize frame and display window
    namedWindow("Task 1 A", CV_WINDOW_AUTOSIZE);
    moveWindow("Task 1 A", 50, 50);
  }


  // critical arrays
  std::vector<uchar> 										features_mask;
  std::vector<cv::Point2f>							features_old;
  std::vector<cv::Point2f>							features_new;
  std::vector<cv::Point2f>							features_old_u;
  std::vector<cv::Point2f>							features_new_u;
  //std::vector<std::vector<cv::Point2f>> features_all;
  //std::vector<std::vector<cv::Point2f>> features_all_u;

  // <<<<<<<<<<
  // <<<<<<<<<<
  // <<<<<<<<<<
  // <<<<<<<<<<

  while (feed)
  {

    // get path of current next image
    std::stringstream path1;
    std::string       path2;
    if(set == "practice")
    {
      path1 << "practice/VO Practice Sequence/" << std::setw(6) << std::setfill('0') << frame_idx << ".png";
    }
    else if (set == "hall")
    {
      path1 << "hall/BYU Hallway Sequence/" << std::setw(6) << std::setfill('0') << frame_idx << ".png";
    }
    else if (set == "urban")
    {
      path1 << "urban/UrbanWithBump." << std::setw(4) << std::setfill('0') << frame_idx+1 << ".tif";
    }
    else
    {
      printf("no image sequence selected \n");
    }
    path2 = path1.str();

    // get current frame
    frame = imread( path2, CV_LOAD_IMAGE_GRAYSCALE);

    // test image validity
    if (!frame.data || frame_idx>ceiling)
    {
      printf("end of image sequence \n");

      // close the file used for saving
      save.close();

      // call c++ plotting wrapper
      // need to read in these lines from a file
      // eventually, I'd like to figure out how to read a .py to a *char
      // rather than feeding in the lines idividually

      Py_SetProgramName(argv[0]);  // optional but recommended
      Py_Initialize();
      PySys_SetArgv(argc, argv);
      PyRun_SimpleString("import matplotlib.pyplot as plt \n"
                         "import numpy as np \n"
                         "A = np.loadtxt('rt.txt') \n"
                         "T_all = np.zeros((3,1)) \n"
                         "for i in range(0, A.shape[0]): \n"
                         "    Tk = np.reshape(A[i], (3,4), 0) # 0 means fill row first \n"
                         "    last = np.zeros((1,4)) \n"
                         "    last[0][3] = 1 # for completing Tk [0 0 0 1] \n"
                         "    Tk = np.vstack((Tk, last)) \n"
                         "    R = Tk[0:3, 0:3] \n"
                         "    T = Tk[0:3, 3] \n"
                         "    T = np.reshape(T, (3,1), 1) # 1 means fill column first \n"
                         "    T_all = np.hstack((T_all, T)) \n"
                         "plt.plot(T_all[0,:], T_all[2,:]) \n"
                         "plt.xlabel('x') \n"
                         "plt.ylabel('y') \n"
                         "plt.title('visual odometry trajectory') \n"
                         "plt.grid(True) \n"
                         "plt.axis('equal') \n"
                         "plt.savefig('trajectory.png') \n"
                         "plt.show() \n");
      Py_Finalize();



      break;
    }



    if (frame_idx!=0)
    {
      // track features
      // features_old are the feature locations for the last frame


      features_mask.clear();
      features_new.clear();


      if (intensity)
      {
        bool edge = false;
        // template matching
        int d_wind = 81; // window dimension
        int d_temp = 31; // template dimension

        for (int i=0; i<features_old.size(); i++)
        {

          // create a window from current image for the current feature
          int x1 = features_old[i].x - (d_wind-1)/2;
          x1 = std::max(x1, 0);
          x1 = std::min(x1, frame.cols-d_wind);
          int y1 = features_old[i].y - (d_wind-1)/2;
          y1 = std::max(y1, 0);
          y1 = std::min(y1, frame.rows-d_wind);
          Mat win = frame(Rect(x1, y1, d_wind, d_wind));

          // create a template from previous image for the current feature
          int x2 = features_old[i].x - (d_temp-1)/2;
          int y2 = features_old[i].y - (d_temp-1)/2;
          if (x2 < 0 || x2 > frame.cols-d_temp || y2 < 0 || y2 > frame.rows-d_temp)
          {
            // template doesn't fit in frame, it's too close to the edge
            // mark it as a bad feature and enter a false location
            features_mask.push_back(0);
            // feature will be removed from both old and new, so use placeholder
            features_new.push_back(features_old[i]);
            edge = true;
          }
          else
          {
            // feature location is good, not too close to the edge
            // mark it as a good feature and perform template matching
            features_mask.push_back(1);

            // generate good template
            Mat tem = frame_old(Rect(x2, y2, d_temp, d_temp));

            // match the current template and window
            Mat output;
            matchTemplate(win, tem, output, TM_CCORR_NORMED);

            // normalize the intensity output
            normalize(output, output, 0, 1, NORM_MINMAX,  -1, Mat());

            // locate the position of highest correlation
            Point max_point;
            minMaxLoc(output, 0, 0, 0, &max_point, Mat());

            // represent the max point in original image coordinates
            max_point.x = max_point.x + d_temp/2 + x1;
            max_point.y = max_point.y + d_temp/2 + y1;

            features_new.push_back(max_point);
          }
        } // end of looping through features

        // remove the features with poor positioning if any were found
        if (edge)
        {
          cleanFeatures(features_old, features_new, features_mask);
        }
      }
      else if (lkflow)
      {
        // generate features_new using lk optical flow

        // perform optical flow

        features_mask.clear();
        std::vector<float> err;
        TermCriteria criteria = TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, 0.01);

        double minEigThreshold = 0.001; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<TODO
        calcOpticalFlowPyrLK	(	frame_old,     frame,
                                features_old,  features_new,
                                features_mask, err ,
                                Size(31,31),   5,     criteria,
                                OPTFLOW_LK_GET_MIN_EIGENVALS, minEigThreshold);

        // clean up
        cleanFeatures2(features_old, features_new, features_mask, err, minEigThreshold);
        //cleanFeatures(features_old, features_new, features_mask);


      }



      // now we have features_new and features_old, cleaned up
      if (features_new.size() < 30)
      {
        // no features could be tracked
        // set R to identity (no rotation)
        R = Mat::eye(3,3, CV_64F);
        // leave T as value from last iteration (propagation)

      }
      else
      {

        // enough points were tracked
        // use findFundamentalMat to locate the outlier feature points
        int ep_dist = 0.1; // acceptable distance from epipolar line <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<TODO
        double confidence = 0.999; // confidence of correct F matrix (0-1) <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< TODO
        features_mask.clear();
        Mat F = findFundamentalMat(features_old, features_new, FM_RANSAC, ep_dist, confidence, features_mask);
        cleanFeatures(features_old, features_new, features_mask);


        //// get F using the good feature matches << THIS IS EXTRA, COME BACK TO THIS. print both and compare TODO
        //Mat F = findFundamentalMat(features_old, features_new, FM_8POINT);

        // undistort
        undistortPoints(features_old, features_old_u, intrinsic, noArray(), noArray(), intrinsic); // try with or without intrinsic again also try putting this before F
        undistortPoints(features_new, features_new_u, intrinsic, noArray(), noArray(), intrinsic); // try with or without intrinsic again also try putting this before F


        // use intrinsic parameters to get essential matrix
        Mat E = intrinsic.t()*F*intrinsic;

        // normalize using svd
        Mat w, u, vt, w2;
        SVD::compute(E, w, u, vt, 0);
        w2 = Mat::eye(3,3, CV_64F);     // 3x3 identity
        w2.at<double>(2,2) = 0;         // new normalized singular values
        E = u * w2 * vt;

        // get focal lengths and principal point
        double fx = intrinsic.at<double>(0,0);
        double fy = intrinsic.at<double>(1,1);
        double cx = intrinsic.at<double>(0,2);
        double cy = intrinsic.at<double>(1,2);

        // decomposing the essential matrix gives us 4 combinations of possible
        // R and T; recoverPose does the cheirality check to get the correct one

        //recoverPose(E, features_old_u, features_new_u, R, T, fx, Point2f(cx, cy)); // TODO use 1 for fx and 0,0 for center for undistorted points
        recoverPose(E, features_old_u, features_new_u, R, T, 1.0, Point2f(0, 0), noArray());

        //std::cout << T << std::endl;
        //std::cout << T.at<double>(2,0) << std::endl;
        if (T.at<double>(2) > 0)
        {
         //std::cout << T << std::endl;
         //std::cout << "greater than zero" << std::endl;
         T = -1* T;
         //std::cout << T << std::endl;
        }


        if (R.at<double>(0,0) < 0 || R.at<double>(1,1) < 0 || R.at<double>(2,2) < 0)
        {
          // the R was poorly resolved from recoverPose
          // set it to identity
          R = Mat::eye(3,3, CV_64F);
        }


        // scale t here TODO
      }

      // bottom row of Tk matrix
      Mat lower(Size(4,1), CV_64FC1);
      lower.at<double>(0,0) = 0;
      lower.at<double>(0,1) = 0;
      lower.at<double>(0,2) = 0;
      lower.at<double>(0,3) = 1;

      // Get Tk for frame and frame_old
      Mat Tk;
      hconcat(R, T, Tk);
      vconcat(Tk, lower, Tk);

      //std::cout << "---" << std::endl;
      //std::cout << R << std::endl;
      //std::cout << T << std::endl;
      //std::cout << lower << std::endl;
      //std::cout << Tk << std::endl;

      Tk = Tk.inv(DECOMP_LU);

      // update Ck
      Ck = Ck*Tk;

      //std::cout << "---" << std::endl;
      //std::cout << Ck << std::endl;

      // output the current Ck matrix to the rt.txt if not the first frame

      save << Ck.at<double>(0,0) << "\t";
      save << Ck.at<double>(0,1) << "\t";
      save << Ck.at<double>(0,2) << "\t";
      save << Ck.at<double>(0,3) << "\t";
      save << Ck.at<double>(1,0) << "\t";
      save << Ck.at<double>(1,1) << "\t";
      save << Ck.at<double>(1,2) << "\t";
      save << Ck.at<double>(1,3) << "\t";
      save << Ck.at<double>(2,0) << "\t";
      save << Ck.at<double>(2,1) << "\t";
      save << Ck.at<double>(2,2) << "\t";
      save << Ck.at<double>(2,3) << "\n";
    }

    // use current frame to refresh features

    /*
    // used for no refresh (for testing feature tracking)
    if (frame_idx==0)
    {
      // find goodFeaturesToTrack and overwrite features_old
      int max_points = 1000;
      double quality = 0.01;
      double min_dist = 10;
      Mat mask;
      int blockSize = 3;
      bool useHarris = false;
      double k = 0.04;
      goodFeaturesToTrack(frame, features_old, max_points, quality, min_dist, mask, blockSize, useHarris, k);
    }
    else
    {
      features_old.clear();
      features_old = features_new;
    }
    */
    features_old.clear();
    int max_points = 500;
    double quality = 0.01; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    double min_dist = 10;
    //Mat mask;
    //int blockSize = 3;
    //bool useHarris = false;
    //double k = 0.04;
    goodFeaturesToTrack(frame, features_old, max_points, quality, min_dist);  // mask, blockSize, useHarris, k


    // remember this frame as the old one next iteration
    frame_old = frame.clone();

    if (live_plotting)
    {
      // display the refreshed features
      Mat frame_draw = frame.clone();
      drawFeatures(frame_draw, features_old);

      // display the image with drawn features
      imshow("Task 1 A", frame_draw);

      // wait for key input from user
      int key = waitKey(1);
      if (key == 110) {
        // the 'n' (next) key was pressed
      } else if (key == 27) {
        // the 'esc' key was pressed, end application
        std::cout << "terminated by user" << std::endl;
        return -1;
      }
    }



    frame_idx++;
  } // end of while loop



  printf("finished \n");
  return 0;
}
