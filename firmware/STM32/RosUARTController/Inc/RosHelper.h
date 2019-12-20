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

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

class RosHelper {
private:
	ros::NodeHandle nh;
	std_msgs::String str_msg;
	ros::Publisher chatter;



public:
	RosHelper();
	virtual ~RosHelper();
	void setupRos(void);
	void RosTask();
	void flush(void);
	void reset_buf(void);
	void rosLoop(void);
};

#endif /* ROSHELPER_H_ */
