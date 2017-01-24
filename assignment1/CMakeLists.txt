cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11) # needed for std::to_string function
project( assignment )
find_package( OpenCV REQUIRED )
add_executable( assignment main.cpp )
target_link_libraries( assignment ${OpenCV_LIBS} )
