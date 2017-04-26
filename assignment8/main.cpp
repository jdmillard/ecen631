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



int main(int argc, char** argv )
{




  // determines when to stop the algorithm
  bool feed = true;

  // whether or not to use practice images
  std::string set;
  set = "practice";
  //set = "hall";
  //set = "urban";

  // decide what to do about camera parameters based on selected image sequence

  // set first frame index
  int frame_idx = 0;

  // // set up writing
  // // Declare what you need
  // FileStorage file12("some_name.txt", FileStorage::WRITE);
  // Mat someMatrixOfAnyType(Size(1,1), CV_64FC1);
  // std::cout << "here" << std::endl;
  // someMatrixOfAnyType.at<double>(0,0) = 5.1;
  // std::cout << "here" << std::endl;
  // // Write to file!
  // file12 << someMatrixOfAnyType;

  std::ofstream save;
  save.open("rt.txt"); // eventually switch to rt.txt


  // initialize frame and display window
  Mat frame;
  namedWindow("Task 1 A", CV_WINDOW_AUTOSIZE);
  moveWindow("Task 1 A", 50, 50);

  // critical arrays
  //std::vector<uchar> 										features_mask;
  std::vector<cv::Point2f>							features_old;
  std::vector<cv::Point2f>							features_new;
  //std::vector<cv::Point2f>							features_new_u;
  std::vector<std::vector<cv::Point2f>> features_all;
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
    if (!frame.data || frame_idx > 10) // REMOVE SECOND HALF OF THIS, IT'S FOR TESTING <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
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



    // track features from previous frame to here
    // get rotation, translation for the current index
    // store transformation, get cumulative transformation and store in rt.txt
    // get new features with gftt for the next frame



    if (frame_idx!=0)
    {
      // track features
      // features_old are the feature locations for the last frame

      // find fundamental matrix

      // get r and t

      // generate Tk


      // output the current Tk matrix to the rt.txt if not the first frame

      // create DUMMY Tk DUMMY Tk DUMMY Tk DUMMY Tk DUMMY Tk
      Mat Tk(Size(4,4), CV_64FC1);
      Tk.at<double>(0,0) = 1;
      Tk.at<double>(0,1) = 2;
      Tk.at<double>(0,2) = 3;
      Tk.at<double>(0,3) = 4;
      Tk.at<double>(1,0) = 5;
      Tk.at<double>(1,1) = 6;
      Tk.at<double>(1,2) = 7;
      Tk.at<double>(1,3) = 8;
      Tk.at<double>(2,0) = 9;
      Tk.at<double>(2,1) = 10;
      Tk.at<double>(2,2) = 11;
      Tk.at<double>(2,3) = 12;
      Tk.at<double>(3,0) = 13;
      Tk.at<double>(3,1) = 14;
      Tk.at<double>(3,2) = 15;
      Tk.at<double>(3,3) = 16;

      // save dummy Tk
      save << Tk.at<double>(0,0) << "\t";
      save << Tk.at<double>(0,1) << "\t";
      save << Tk.at<double>(0,2) << "\t";
      save << Tk.at<double>(0,3) << "\t";
      save << Tk.at<double>(1,0) << "\t";
      save << Tk.at<double>(1,1) << "\t";
      save << Tk.at<double>(1,2) << "\t";
      save << Tk.at<double>(1,3) << "\t";
      save << Tk.at<double>(2,0) << "\t";
      save << Tk.at<double>(2,1) << "\t";
      save << Tk.at<double>(2,2) << "\t";
      save << Tk.at<double>(2,3) << "\n";
    }

    // use current frame to refresh features

    // find goodFeaturesToTrack
    int max_points = 1000;
    double quality = 0.01;
    double min_dist = 10;
    Mat mask;
    int blockSize = 3;
    bool useHarris = false;
    double k = 0.04;
    goodFeaturesToTrack(frame, features_old, max_points, quality, min_dist, mask, blockSize, useHarris, k);



    // display the refreshed features
    drawFeatures(frame, features_old);


    // display image
    imshow("Task 1 A", frame);


    // wait for key input from user
    int key = waitKey();
    if (key == 110) {
      // the 'n' (next) key was pressed
    } else if (key == 27) {
      // the 'esc' key was pressed, end application
      std::cout << "terminated by user" << std::endl;
      return -1;
    }



    frame_idx++;
  } // end of while loop



  printf("finished \n");
  return 0;
}
