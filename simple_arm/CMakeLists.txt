cmake_minimum_required(VERSION 2.8.3)
project(simple_arm)

find_package(catkin REQUIRED COMPONENTS
	std_msgs
	message_generation
	controller_manager
)

add_compile_options(-std=c++11)

## Generate services in the 'srv' folder
add_service_files(
   FILES
   GoToPosition.srv
)

## Generate added messages and services with any dependencies listed here
generate_messages(
   DEPENDENCIES
   std_msgs  # Or other packages containing msgs
)

catkin_package()

include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(simple_mover src/simple_mover.cpp)
target_link_libraries(simple_mover ${catkin_LIBRARIES})
add_dependencies(simple_mover simple_arm_generate_messages_cpp)

add_executable(arm_mover src/arm_mover.cpp)
target_link_libraries(arm_mover ${catkin_LIBRARIES})
add_dependencies(arm_mover simple_arm_generate_messages_cpp)

add_executable(look_away src/look_away.cpp)
target_link_libraries(look_away ${catkin_LIBRARIES})
add_dependencies(look_away simple_arm_generate_messages_cpp)
