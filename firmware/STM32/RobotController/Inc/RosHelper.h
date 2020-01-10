/*
 * RosHelper.h
 *
 *  Created on: Dec 17, 2019
 *      Author: developer
 */

#ifndef ROSHELPER_H_
#define ROSHELPER_H_

#include <ros.h>
#include "config.h"
#include <std_msgs/String.h>
#include "WheelSubscriber.h"
#include "WheelPublisher.h"

class RosHelper {
private:
	ros::NodeHandle nh;
	std_msgs::String str_msg;
	ros::Publisher chatter;
	WheelSubscriber *wheel;
	WheelSubscriber *wheel2;
	WheelPublisher *encoder;
	WheelPublisher *encoder2;

	void rosLoop(void);
public:
	RosHelper();
	virtual ~RosHelper();
	void setupRos(UartHelper *uart_helper,TIM_HandleTypeDef *main_htim, TIM_HandleTypeDef *main_htim2, TIM_HandleTypeDef *encoder_htim, TIM_HandleTypeDef *encoder_htim2);
	void RosTask();
	void setSpeedTask(void);
	void setSpeedTask2(void);
	void readSpeedTask(void);
	void readSpeedTask2(void);
	void exti_Callback(uint16_t GPIO_Pin);
};

#endif /* ROSHELPER_H_ */
