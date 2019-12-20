/*
 * RosHelper.cpp
 *
 *  Created on: Dec 17, 2019
 *      Author: developer
 */

#include "RosHelper.h"

RosHelper::RosHelper():chatter("chatter",&str_msg) {
	// TODO Auto-generated constructor stub

}

RosHelper::~RosHelper() {
	// TODO Auto-generated destructor stub
}

void RosHelper::setupRos(void)
{
  nh.initNode();
  nh.advertise(chatter);
}


void RosHelper::rosLoop(void)
{
  str_msg.data = "Hello world!";
  chatter.publish(&str_msg);
  nh.spinOnce();
  osDelay(500);
}

void RosHelper::RosTask(void){

	for(;;){
		rosLoop();
	}
}

void RosHelper::flush(void){
	nh.getHardware()->flush();
}

void RosHelper::reset_buf(void){
	nh.getHardware()->reset_rbuf();
}

