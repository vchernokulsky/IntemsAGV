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

void RosHelper::setupRos(TIM_HandleTypeDef *main_htim)
{
  nh.initNode();
  nh.advertise(chatter);

  wheel = new WheelSubscriber("my_robot/left_wheel_vel");
  wheel->set_ports(GPIOA, GPIOC, GPIOA, GPIOA);
  wheel->set_pins(GPIO_PIN_9, GPIO_PIN_7, GPIO_PIN_0, GPIO_PIN_1);

//    wheel->set_ports(GPIOA, GPIOB, GPIOC, GPIOC);
//    wheel->set_pins(GPIO_PIN_11, GPIO_PIN_6, GPIO_PIN_6, GPIO_PIN_9);
//  wheel->subscribe(&nh, main_htim, TIM_CHANNEL_1);
  wheel->subscribe(&nh, main_htim, TIM_CHANNEL_1, TIM_CHANNEL_2);
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

void RosHelper::setSpeedTask(void){
	for(;;){
		wheel->set_speed();
		osDelay(50);
	}
}

void RosHelper::flush(void){
	nh.getHardware()->flush();
}

void RosHelper::reset_buf(void){
	nh.getHardware()->reset_rbuf();
}

