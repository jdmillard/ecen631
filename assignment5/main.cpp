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

  Mat image_a, image_b;
  int gap = 3; // the gap between images in sequence

  namedWindow("Task 1 A", CV_WINDOW_AUTOSIZE);
  namedWindow("Task 1 B", CV_WINDOW_AUTOSIZE);
  moveWindow("Task 1 A", 50, 50);
  moveWindow("Task 1 B", 700, 50);


  // cycle through image set types
  for (int i=0; i<5; i++){
    // cycle through the images of each set
    for (int j=1; j<= nn[i]; j++){

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
        // display images a and b
        imshow("Task 1 A", image_a);
        imshow("Task 1 B", image_b);


        // perform operations here between a and b




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








    }

  }


  printf("finished \n");
  return 0;
}
