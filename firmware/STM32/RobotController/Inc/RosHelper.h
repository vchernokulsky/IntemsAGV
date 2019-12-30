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

class RosHelper {
private:
	ros::NodeHandle nh;
	std_msgs::String str_msg;
	ros::Publisher chatter;


	void rosLoop(void);
public:
	RosHelper();
	virtual ~RosHelper();
	void setupRos(UartHelper *uart_helper);
	void RosTask();
};

#endif /* ROSHELPER_H_ */
