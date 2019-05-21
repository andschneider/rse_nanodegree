#!/bin/bash

xterm -e "source devel/setup.bash; roslaunch turtlebot_gazebo turtlebot_world.launch" &
sleep 5

xterm -e "roslaunch turtlebot_gazebo amcl_demo.launch" &
sleep 5

xterm -e "roslaunch home_service_robot view_navigation.launch" &
sleep 5

xterm -e "rosrun pick_objects pick_objects"

