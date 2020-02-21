#!/bin/bash
source /opt/ros/melodic/setup.bash
export ROS_IP=192.168.2.150
export ROS_HOSTNAME=192.168.2.150
export ROS_MASTER_URI=http://192.168.2.150:11311
rosrun map_server map_saver -f ~/map
