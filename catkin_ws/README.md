### simple_arm is a 2 DOF arm with a camera as it's end effector

#### simple_mover is c++ script, which becomes a ROS node, that moves the arm's two joints.

build the simple_mover script and package:
```sh
$ cd ~/rse_nanodegree/catkin_ws/
$ catkin_make
```

#### run simple_mover
First, launch the simple_arm in gazebo using ROS.
```sh
$ cd ~/rse_nanodegree/catkin_ws/
$ source devel/setup.bash
$ roslaunch simple_arm robot_spawn.launch
```

and to finally make the arm move, open a new terminal:
```sh
$ cd ~/rse_nanodegree/catkin_ws/
$ source devel/setup.bash
$ rosrun simple_arm simple_mover
```

![alt text](/src/simple_arm/images/sim_example.png)
