#!/bin/bash

xterm -e "source devel/setup.bash; roslaunch home_service_robot turtlebot.launch" &
sleep 5

xterm -e "roslaunch home_service_robot amcl.launch" &
sleep 5

xterm -e "roslaunch home_service_robot view_navigation.launch" &
sleep 5

xterm -e "rosrun add_markers add_markers" &
sleep 5

xterm -e "rosrun pick_objects pick_objects"

