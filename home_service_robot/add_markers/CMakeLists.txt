cmake_minimum_required(VERSION 2.8.3)
project(add_markers)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  visualization_msgs
)

include_directories(${catkin_INCLUDE_DIRS})

add_executable(add_markers src/add_markers.cpp)
target_link_libraries(add_markers ${catkin_LIBRARIES})
