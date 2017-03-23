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


////////    ///     //////  //    //       //
   //      // //   //    // //   //      ////
   //     //   //  //       //  //         //
   //    //     //  //////  /////          //
   //    /////////       // //  //         //
   //    //     // //    // //   //        //
   //    //     //  //////  //    //     //////


  // initialize variables and windows
  

  printf("finished \n");
  return 0;
}
