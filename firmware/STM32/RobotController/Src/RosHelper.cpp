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

void RosHelper::setupRos(UartHelper *uart_helper, TIM_HandleTypeDef *main_htim,  TIM_HandleTypeDef *main_htim2, TIM_HandleTypeDef *encoder_htim, TIM_HandleTypeDef *encoder_htim2)
{
	nh.initNode();
	nh.advertise(chatter);
	nh.setUARTHelper(uart_helper);

	wheel = new WheelSubscriber("my_robot/right_wheel_vel");
	wheel->set_pins(GPIO_REN1, PIN_REN1, GPIO_LEN1, PIN_LEN1);
	wheel->set_timers(main_htim, CHANNEL1, CHANNEL_REV1);
//	wheel->subscribe(&nh);

	wheel2 = new WheelSubscriber("my_robot/left_wheel_vel");
	wheel2->set_pins(GPIO_REN2, PIN_REN2, GPIO_LEN2, PIN_LEN2);
	wheel2->set_timers(main_htim2, CHANNEL2, CHANNEL_REV2);
//	wheel2->subscribe(&nh);

	cmd_vel = new CmdVelSubscriber(&nh, wheel2, wheel);

	encoder = new WheelPublisher(&nh, "/my_robot/right_wheel_angle", encoder_htim, uart_helper);
	encoder2 = new WheelPublisher(&nh, "/my_robot/left_wheel_angle", encoder_htim2, uart_helper);

}


void RosHelper::rosLoop(void)
{
  str_msg.data = "Hello world!";
  chatter.publish(&str_msg);
  encoder->publish();
  encoder2->publish();
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
		float cur_speed = encoder->get_speed();
		wheel->set_speed(cur_speed);
		osDelay(1);
	}
}
void RosHelper::setSpeedTask2(void){
	for(;;){
		float cur_speed = encoder2->get_speed();
		wheel2->set_speed(cur_speed);
		osDelay(1);
	}
}


