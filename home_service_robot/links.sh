#!/bin/bash

REPO_DIR=~/Github/rse_nanodegree/home_service_robot
CATKIN_DIR=~/rse_nd/catkin_ws/src

mkdir -p $REPO_DIR/add_markers/src 
touch $REPO_DIR/add_markers/src/add_markers.cpp
ln -s $REPO_DIR/add_markers $CATKIN_DIR/add_markers

mkdir $REPO_DIR/map
ln -s $REPO_DIR/map $CATKIN_DIR/map

mkdir -p $REPO_DIR/pick_objects/src 
touch $REPO_DIR/pick_objects/src/pick_objects.cpp
ln -s $REPO_DIR/pick_objects $CATKIN_DIR/pick_objects

mkdir $REPO_DIR/rvizConfig
ln -s $REPO_DIR/rvizConfig $CATKIN_DIR/rvizConfig

ln -s $REPO_DIR/scripts $CATKIN_DIR/scripts

