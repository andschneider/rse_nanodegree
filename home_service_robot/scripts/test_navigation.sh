#!/bin/bash

CATKIN_SRC=~/rse_nd/catkin_ws/src


xterm -e "source devel/setup.bash; roslaunch turtlebot_gazebo turtlebot_world.launch" &
sleep 5

xterm -e "roslaunch turtlebot_gazebo amcl_demo.launch" &
sleep 5

xterm -e "roslaunch turtlebot_interactions/turtlebot_rviz_launchers/launch/view_navigation.launch"


