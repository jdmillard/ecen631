#include <opencv2/opencv.hpp>
#include <string> // needed for setting std::strings and using to_string
#include <fstream>

#include <Python.h>

using namespace cv;





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

  // initialize frame and display window
  Mat frame;
  namedWindow("Task 1 A", CV_WINDOW_AUTOSIZE);
  moveWindow("Task 1 A", 50, 50);

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
    if (!frame.data || frame_idx > 10) // REMOVE SECOND HALF OF THIS, IT'S FOR TESTING
    {
      printf("end of image sequence \n");
      // write the txt file with r and t
      // confirm that it is written, and closed

      // for now, assume a text file named rt.txt was just generated
      // if it's not there, copy the one in "practice" which is a downloaded
      // folder, not commited

      // display truth (if exists)

      // call c++ plotting wrapper
      // need to read in these lines from a file

      //Py_SetProgramName(argv[0]);  /* optional but recommended */
      Py_Initialize();
      PySys_SetArgv(argc, argv); // must call this to get sys.argv and relative imports
      PyRun_SimpleString("import matplotlib.pyplot as plt \n"
                         "import numpy as np \n"
                         "A = np.loadtxt('rt.txt') \n"
                         "print(A.shape) \n"
                         "print(A[0][0]) \n"
                         "print(A[0][1]) \n"
                         "t = np.arange(0.0, 2.0, 0.01) \n"
                         "s = 1 + np.sin(2*np.pi*t) \n"
                         "plt.plot(t, s) \n"
                         "plt.xlabel('time (s)') \n"
                         "plt.ylabel('voltage (mV)') \n"
                         "plt.title('About as simple as it gets, folks') \n"
                         "plt.grid(True) \n"
                         "plt.show() \n"
                         "print('did it work?')\n");
      Py_Finalize();



      break;
    }

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
