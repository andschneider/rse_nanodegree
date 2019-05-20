#!/bin/bash

xterm -e "source devel/setup.bash; roslaunch turtlebot_gazebo turtlebot_world.launch" &
sleep 5

xterm -e "roslaunch turtlebot_gazebo amcl_demo.launch initial_pose_a:=0.0" &
sleep 5

xterm -e "roslaunch home_service_robot view_navigation.launch"

