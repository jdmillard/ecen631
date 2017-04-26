#include <opencv2/opencv.hpp>
#include <string> // needed for setting std::strings and using to_string
#include <fstream>

using namespace cv;





int main(int argc, char** argv )
{

  // determines when to stop the algorithm
  bool feed = true;

  // whether or not to use practice images
  bool practice = true;

  // set first frame index according to dataset naming
  int frame_idx;
  if(practice)
  {
    frame_idx = 0;
  }
  else
  {
    frame_idx = 1;
  }

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
    if(practice)
    {
      path1 << "practice/VO Practice Sequence/" << std::setw(6) << std::setfill('0') << frame_idx << ".png";
    }
    else
    {
      path1 << "urban/UrbanWithBump." << std::setw(4) << std::setfill('0') << frame_idx << ".png";
    }
    path2 = path1.str();

    // get current frame
    frame = imread( path2, CV_LOAD_IMAGE_GRAYSCALE);

    // test image validity
    if (!frame.data)
    {
      printf("end of image sequence \n");
      // run the write of txt file with r and t
      // call c++ plotting wrapper
      // perhaps display truth
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
