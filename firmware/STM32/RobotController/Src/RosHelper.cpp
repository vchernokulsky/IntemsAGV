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

void RosHelper::setupRos(UartHelper *uart_helper, TIM_HandleTypeDef *main_htim,  TIM_HandleTypeDef *main_htim2)
{
  nh.initNode();
  nh.advertise(chatter);
  nh.setUARTHelper(uart_helper);

  	wheel = new WheelSubscriber("my_robot/left_wheel_vel");
	wheel->set_ports(GPIOA, GPIOC, GPIOA, GPIOA);
	wheel->set_pins(GPIO_PIN_9, GPIO_PIN_7, GPIO_PIN_0, GPIO_PIN_1);
	wheel->subscribe(&nh, main_htim, TIM_CHANNEL_1, TIM_CHANNEL_2);

  wheel2 = new WheelSubscriber("my_robot/right_wheel_vel");
  wheel2->set_ports(GPIOA, GPIOB, GPIOC, GPIOC);
  wheel2->set_pins(GPIO_PIN_11, GPIO_PIN_6, GPIO_PIN_6, GPIO_PIN_9);
  wheel2->subscribe(&nh, main_htim2, TIM_CHANNEL_1, TIM_CHANNEL_4);
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
		osDelay(5);
	}
}
void RosHelper::setSpeedTask2(void){
	for(;;){
		wheel2->set_speed();
		osDelay(5);
	}
}


