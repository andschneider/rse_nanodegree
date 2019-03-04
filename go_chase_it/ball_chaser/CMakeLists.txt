cmake_minimum_required(VERSION 2.8.3)
project(ball_chaser)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  image_transport
  message_generation
  roscpp
  sensor_msgs
  std_msgs
)

find_package(OpenCV REQUIRED)

## Generate services in the 'srv' folder
add_service_files(
  FILES
  DriveToTarget.srv
)

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package()

include_directories(include ${OpenCV_INCLUDE_DIRS} ${catkin_INCLUDE_DIRS} )

add_executable(drive_bot src/drive_bot.cpp)
target_link_libraries(drive_bot ${catkin_LIBRARIES})
add_dependencies(drive_bot ball_chaser_generate_messages_cpp)

add_executable(process_image src/process_image.cpp)
target_link_libraries(process_image ${OpenCV_LIBRARIES} ${catkin_LIBRARIES})
add_dependencies(process_image ball_chaser_generate_messages_cpp)
