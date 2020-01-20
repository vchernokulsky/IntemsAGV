/*
 * RosHelper.cpp
 *
 *  Created on: Dec 17, 2019
 *      Author: developer
 */

#include "RosHelper.h"

//static const char test_string[] =
//		"12345678901234567890123456789012345678901234567890"
//		"12345678901234567890123456789012345678901234567890"
//		"12345678901234567890123456789012345678901234567890"
//		"12345678901234567890123456789012345678901234567890"
//		"12345678901234567890123456789012345678901234567890"
//		"12345678901234567890123456789012345678901234567890"
//		"12345678901234567890123456789012345678901234567890"
//		"12345678901234567890123456789012345678901234567890"
//		"12345678901234567890123456789012345678901234567890"
//		"12345678901234567890123456789012345678901234567890";
static const char test_string[] = "Hello world";

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

	//===Right wheel===
	wheel = new WheelSubscriber();
	wheel->set_pins(GPIO_REN1, PIN_REN1, GPIO_LEN1, PIN_LEN1);
	wheel->set_timers(main_htim, CHANNEL1, CHANNEL_REV1);

	//===Left wheel===
	wheel2 = new WheelSubscriber();
	wheel2->set_pins(GPIO_REN2, PIN_REN2, GPIO_LEN2, PIN_LEN2);
	wheel2->set_timers(main_htim2, CHANNEL2, CHANNEL_REV2);

	//===Right encoder===
	encoder = new WheelPublisher(encoder_htim, uart_helper);

	//===Left encoder===
	encoder2 = new WheelPublisher(encoder_htim2, uart_helper);

	cmd_vel = new CmdVelSubscriber(&nh, wheel2, wheel);
	odom = new OdometryPublisher(&nh, encoder2, encoder);

}


void RosHelper::rosLoop(void)
{
	str_msg.data = test_string;
	chatter.publish(&str_msg);
	odom->publish();
	nh.spinOnce();
	osDelay(ROS_SPINONCE_DELAY);
}

void RosHelper::RosTask(void)
{
	for(;;)
	{
		rosLoop();
	}
}

void RosHelper::setSpeedTask(void)
{
	for(;;)
	{
		wheel->set_speed(encoder->get_speed());
		osDelay(SET_SPEED_DELAY);
	}
}

void RosHelper::setSpeedTask2(void)
{
	for(;;)
	{
		wheel2->set_speed(encoder2->get_speed());
		osDelay(SET_SPEED_DELAY);
	}
}

void RosHelper::encoderTask(void)
{
	for(;;)
	{
		encoder->tick_calculate();
		osDelay(GET_TICK_DELAY);
	}
}

void RosHelper::encoderTask2(void)
{
	for(;;)
	{
		encoder2->tick_calculate();
		osDelay(GET_TICK_DELAY);
	}
}


