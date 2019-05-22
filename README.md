# Robotics Software Engineer Nanodegree from Udacity


[![Udacity - Robotics NanoDegree Program](https://s3-us-west-1.amazonaws.com/udacity-robotics/Extra+Images/RoboND_flag.png)](https://www.udacity.com/robotics)

---

This repo is a collection of my work to earn a nanodegree from Udacity in Robo   tics Software Engineering. The course page with more information and the syllabus is [here](https://www.udacity.com/course/robotics-software-engineer--nd209).

I've always had an offshoot interest in robotics (I almost studied it in college) so this course was an attempt to gain an introduction to the field. In addition, I wanted to learn some C++ and to continue getting more comfortable using Ubuntu and Vim.

## Personal Goals

1. Learn the basics of C++
2. Learn some algorithms used in robotics, such as SLAM
3. Get better at Vim
4. Get more comfortable using Linux

## Course Projects

1. __Build My World__: introduction to Gazebo.
2. [__Go Chase It!__](https://github.com/andschneider/rse_nanodegree/tree/master/go_chase_it): build a robot that chases colored balls.
3. __Where am I?__: use Monte Carlo Localization to estimate a robot's position.
4. __Map My World__: use RTAB-Map to localize a robot and create 2D and 3D maps of the environment.
5. [__Home Service Robot__](https://github.com/andschneider/rse_nanodegree/tree/master/home_service_robot): create a home service robot that will autonomously map an environment and navigate to different locations, picking up and dropping off objects. 

## Course Highlights

 - My personal favorite project was [Go Chase It!](https://github.com/andschneider/rse_nanodegree/tree/master/go_chase_it). This is largely because it involved the most C++ and I got to learn some OpenCV (which was not required). I also was able to learn some basic image processing, such as thresholding and edge detection.

 - Graph search algorithms like A*

 - Kalman filter and extended Kalman filter
 
 - FastSLAM
 
 - Learned a useful `grep` command for string searching in directories:

    ```bash
    $ grep --recursive "search-term" *
    ```

 - Learned about `clang-format` for formatting C++ files:
 
    ```bash
    $ clang-format file.cpp -style "google" -i
    ```