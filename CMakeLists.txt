cmake_minimum_required(VERSION 3.13)

# set the project name and version
project (learning_opencv)

set (CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED) 

include_directories(${OpenCV_INCLUDE_DIRS}) 
add_executable(cv_app main.cpp)
target_link_libraries(cv_app ${OpenCV_LIBS})
