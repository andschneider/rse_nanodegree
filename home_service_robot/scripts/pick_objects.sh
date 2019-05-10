#!/bin/bash

xterm -e "source devel/setup.bash; roslaunch turtlebot_gazebo turtlebot_world.launch" &
sleep 5

xterm -e "roslaunch turtlebot_gazebo amcl_demo.launch" &
sleep 5

xterm -e "roslaunch turtlebot_interactions/turtlebot_rviz_launchers/launch/view_navigation.launch" &
sleep 5

xterm -e "~/rse_nd/catkin_ws/build/pick_objects/pick_objects"

