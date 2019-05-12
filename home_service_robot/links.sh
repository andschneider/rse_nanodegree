#!/bin/bash

REPO_DIR=~/Github/rse_nanodegree/home_se
CATKIN_DIR=~/rse_nd/catkin_ws/src

catkin_create_pkg add_markers roscpp visualization_msgs
ln -s $REPO_DIR/add_markers $CATKIN_DIR/add_markers

mkdir $REPO_DIR/map
ln -s $REPO_DIR/map $CATKIN_DIR/map

catkin_create_pkg pick_objects roscpp visualization_msgs
ln -s $REPO_DIR/pick_objects $CATKIN_DIR/pick_objects

mkdir $REPO_DIR/rvizConfig
ln -s $REPO_DIR/rvizConfig $CATKIN_DIR/rvizConfig

ln -s $REPO_DIR/scripts $CATKIN_DIR/scripts

