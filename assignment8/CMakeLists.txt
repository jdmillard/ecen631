cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11) # needed for std::to_string function
project( assignment )
find_package( OpenCV REQUIRED )
add_executable( assignment main.cpp )
target_link_libraries( assignment ${OpenCV_LIBS} )

# used to embed python script
find_package(PythonLibs 2.7 REQUIRED) # this has to be 2.7 because the 3.0 API requires argv to be wchar_t** rather than the default char**
include_directories(${PYTHON_INCLUDE_DIRS})
target_link_libraries( assignment ${PYTHON_LIBRARIES})
