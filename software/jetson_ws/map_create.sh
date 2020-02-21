cd IntemsAGV/software/catkin_ws/
export ROS_IP=192.168.2.150
export ROS_HOSTNAME=192.168.2.150
export ROS_MASTER_URI=http://192.168.2.150:11311
source devel/setup.bash
roslaunch cube_robot_slam cube_robot_slam.launch
