#pragma once

/******************** NETWORK ************************/
#define DEFAULT_ROBOT_IP_ADRESS {192, 168, 2, 114}
#define DEFAULT_NETWORK_MASK {255, 255, 255, 0}
#define DEFAULT_GATEAWAY {192, 168, 2, 1}
#define DEFAULT_ROS_CLIENT_PORT 10888
#define DEFAULT_SETUP_SERVER_PORT 11511
/************************************************/

/********************SERVER************************/
#define DEFAULT_ROS_SERIALNODE_IP_ADRESS {192, 168, 2, 102}
#define DEFAULT_ROS_SERIALNODE_PORT 11411
/************************************************/

/***************ROBOT***************************/
#define DEFAULT_CMDVEL_TOPIC "/cmd_vel"
#define DEFAULT_ODOMETRY_TOPIC "/odom"
#define DEFAULT_BASE_FRAME "base_link"
#define DEFAULT_ODOMETRY_FRAME "odom"

#define DEFAULT_MAX_PWD_ALLOWED 255
#define DEFAULT_RAD_PER_TICK 0.0174533
#define DEFAULT_RADIUS 0.1
#define DEFAULT_WHEEL_SEPARATION 0.45

#define DEFAULT_MAX_LIN_SPEED 1.9
#define DEFAULT_ANG_VEL_MAX 3.8
/************************************************/
