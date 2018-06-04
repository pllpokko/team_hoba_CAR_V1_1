#!/bin/bash
source ./devel/setup.bash
hostname –I 1.1.1.2
export ROS_IP=1.1.1.2
rosrun auto_car auto_car
