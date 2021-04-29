cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)

project( RandomForest )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(program main.cpp DecisionTree.cpp forest.cpp Node.cpp)

target_link_libraries( program ${OpenCV_LIBS} )
