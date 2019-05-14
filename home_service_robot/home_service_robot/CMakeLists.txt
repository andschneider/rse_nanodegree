cmake_minimum_required(VERSION 2.8.3)
project(home_service_robot)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

find_package(catkin REQUIRED)

catkin_package()

include_directories(${catkin_INCLUDE_DIRS})

#target_link_libraries(home_service_robot ${catkin_LIBRARIES})

#install(DIRECTORY launch
#        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})

#install(DIRECTORY maps
#        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})

#install(DIRECTORY rvizConfig
#        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})

#install(DIRECTORY worlds
#        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
