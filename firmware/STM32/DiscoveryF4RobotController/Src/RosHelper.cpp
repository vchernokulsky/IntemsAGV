/*
 * RosHelper.cpp
 *
 *  Created on: Dec 17, 2019
 *      Author: developer
 */

#include "RosHelper.h"

static const char test_string[] = "Hello world";

RosHelper::RosHelper():chatter("chatter",&str_msg),wheel(), wheel2() , encoder(), encoder2(), cmd_vel(), odom(){
	// TODO Auto-generated constructor stub

}


RosHelper::~RosHelper() {
	// TODO Auto-generated destructor stub
}

void RosHelper::setupRos(TIM_HandleTypeDef *main_htim,  TIM_HandleTypeDef *main_htim2, TIM_HandleTypeDef *encoder_htim, TIM_HandleTypeDef *encoder_htim2)
{
	nh.initNode();
	nh.advertise(chatter);

	//===Right wheel===
	wheel.set_pins(GPIO_REN1, PIN_REN1, GPIO_LEN1, PIN_LEN1);
	wheel.set_timers(main_htim, CHANNEL1, CHANNEL_REV1);

	//===Left wheel===
	wheel2.set_pins(GPIO_REN2, PIN_REN2, GPIO_LEN2, PIN_LEN2);
	wheel2.set_timers(main_htim2, CHANNEL2, CHANNEL_REV2);

	//===Right encoder===
	encoder.init(encoder_htim);

	//===Left encoder===
	encoder2.init(encoder_htim2);

	cmd_vel.init(&nh, &wheel2, &wheel);
	odom.init(&nh, &encoder2, &encoder);

}

void RosHelper::setupRos()
{
	nh.initNode();
	nh.advertise(chatter);
}


void RosHelper::rosLoop(void)
{
	str_msg.data = test_string;
//	chatter.publish(&str_msg);
	odom.publish();
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
		wheel.set_speed(encoder.get_speed());
		osDelay(SET_SPEED_DELAY);
	}
}

void RosHelper::setSpeedTask2(void)
{
	for(;;)
	{
		wheel2.set_speed(encoder2.get_speed());
		osDelay(SET_SPEED_DELAY);
	}
}

void RosHelper::encoderTask(void)
{
	for(;;)
	{
		encoder.tick_calculate();
		osDelay(GET_TICK_DELAY);
	}
}

void RosHelper::encoderTask2(void)
{
	for(;;)
	{
		encoder2.tick_calculate();
		osDelay(GET_TICK_DELAY);
	}
}

void RosHelper::cmdvelTimeoutTask(){
	for(;;){
		cmd_vel.check_timeout();
		osDelay(CMDVEL_TIMEOUT_DELAY);
	}
}


