#!/bin/bash

CATKIN_SRC=~/rse_nd/catkin_ws/src


xterm -e "source devel/setup.bash; roslaunch turtlebot_gazebo turtlebot_world.launch" &
sleep 5

xterm -e "roslaunch turtlebot_gazebo gmapping_demo.launch" &
sleep 5

xterm -e "roslaunch home_service_robot view_navigation.launch" &
sleep 5

xterm -e "roslaunch turtlebot_teleop keyboard_teleop.launch"

