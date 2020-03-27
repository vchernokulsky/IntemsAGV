/*
 * RosHelper.h
 *
 *  Created on: Dec 17, 2019
 *      Author: developer
 */

#ifndef ROSHELPER_H_
#define ROSHELPER_H_

#include <ros.h>
#include "System_config.h"
#include <std_msgs/String.h>
#include <std_msgs/Int16.h>
#include "WheelSubscriber.h"
#include "WheelPublisher.h"
#include "CmdVelSubscriber.h"
#include "OdometryPublisher.h"


class RosHelper {
private:
	static SetUpHelper *settings;
	ros::NodeHandle nh;
//	std_msgs::String str_msg;
//	ros::Publisher chatter;
	WheelSubscriber wheel;
	WheelSubscriber wheel2;
	WheelPublisher encoder;
	WheelPublisher encoder2;
	CmdVelSubscriber cmd_vel;
	OdometryPublisher odom;

	void rosLoop(void);
public:
	RosHelper();
	virtual ~RosHelper();
	void setupRos(TIM_HandleTypeDef *main_htim, TIM_HandleTypeDef *main_htim2, TIM_HandleTypeDef *encoder_htim, TIM_HandleTypeDef *encoder_htim2, SetUpHelper *main_settings);
	void setupRos();
	void RosTask();
	void setSpeedTask(void);
	void setSpeedTask2(void);
	void encoderTask(void);
	void encoderTask2(void);
	void readSpeedTask(void);
	void readSpeedTask2(void);
	void cmdvelTimeoutTask();
};

#endif /* ROSHELPER_H_ */
