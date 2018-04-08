cmake_minimum_required(VERSION 3.9)
project(openCVScratch)

set(CMAKE_CXX_STANDARD 11)

find_package( OpenCV REQUIRED )

add_executable(openCVScratch main.cpp)
target_link_libraries( openCVScratch ${OpenCV_LIBS} )
