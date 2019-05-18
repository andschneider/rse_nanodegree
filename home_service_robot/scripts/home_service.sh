#!/bin/bash

xterm -e "source devel/setup.bash; roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=$(rospack find home_service_robot)/worlds/myworld.world" &
sleep 5

xterm -e "roslaunch turtlebot_gazebo amcl_demo.launch map_file:=$(rospack find home_service_robot)/maps/map.yaml initial_pose_a:=4.71" &
sleep 5

xterm -e "roslaunch home_service_robot view_navigation.launch" &
sleep 5

xterm -e "rosrun add_markers add_markers" &
sleep 5

xterm -e "rosrun pick_objects pick_objects"

