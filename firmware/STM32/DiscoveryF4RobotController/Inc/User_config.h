#pragma once

#define PID_SETUP false

/********************WIZNET************************/
//#define WIZNET_MAC_ADRESS {0x00, 0x08, 0xdc, 0xab, 0xcd, 0xef}
#define WIZNET_IP_ADRESS {192, 168, 2, 114}
#define WIZNET_MASK {255, 255, 255, 0}
#define WIZNET_GATEAWAY {192, 168, 2, 1}
#define WIZNET_PORT 10888
#define DEFAULT_SETUP_SERVER_PORT 11511

#define WIZNET_CHECK_ERRORS_DELAY 5
#define WIZNET_CHECK_FREEZING_DELAY 100
/************************************************/

/********************SERVER************************/
#define SERVER_IP_ADRESS {192, 168, 2, 150}
#define SERVER_PORT 11411
/************************************************/

/*******************CONTROL****************/
#define ROS_SPINONCE_DELAY 40
#define SET_SPEED_DELAY 100
#define GET_TICK_DELAY 100
#define CMDVEL_TIMEOUT 5000
#define CMDVEL_TIMEOUT_DELAY 100

#define KP_DEFAULT 1.0
#define KI_DEAFAULT 0.0
#define KD_DEFAULT 0.0

#define MAX_PWD 255


#define PWD_STEP_L 10
#define SPEED_DELTA_L 40

#define PWD_STEP_M 5
#define SPEED_DELTA_M 13

#define PWD_STEP_S 1
#define SPEED_DELTA_S 2
/************************************************/

/***************ROBOT***************************/
#define CMDVEL_TOPIC "/cmd_vel"
#define ODOMETRY_TOPIC "/odom"
#define BASE_FRAME "base_link"
#define ODOMETRY_FRAME "odom"

#define DEFAULT_MAX_PWD_ALLOWED 255
#define DEFAULT_RAD_PER_TICK 0.0174533
#define DEFAULT_RADIUS 0.1
#define DEFAULT_WHEEL_SEPARATION 0.45

#define DEFAULT_MAX_LIN_SPEED 1.9
#define DEFAULT_ANG_VEL_MAX 3.8
/************************************************/
