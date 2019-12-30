/*
 * RosHelper.h
 *
 *  Created on: Dec 17, 2019
 *      Author: developer
 */

#ifndef ROSHELPER_H_
#define ROSHELPER_H_

#include <ros.h>
#include <std_msgs/String.h>
#include "WheelSubscriber.h"

class RosHelper {
private:
	ros::NodeHandle nh;
	std_msgs::String str_msg;
	ros::Publisher chatter;
	WheelSubscriber *wheel;


	void rosLoop(void);
public:
	RosHelper();
	virtual ~RosHelper();
	void setupRos(UartHelper *uart_helper,TIM_HandleTypeDef *main_htim);
	void RosTask();
	void setSpeedTask(void);
};

#endif /* ROSHELPER_H_ */
