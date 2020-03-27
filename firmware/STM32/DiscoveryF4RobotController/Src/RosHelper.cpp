/*
 * RosHelper.cpp
 *
 *  Created on: Dec 17, 2019
 *      Author: developer
 */
#include "SetUpHelper.h"
#include "RosHelper.h"

SetUpHelper *RosHelper::settings = nullptr;

RosHelper::RosHelper(): wheel(), wheel2() , encoder(), encoder2(), cmd_vel(), odom(){
	// TODO Auto-generated constructor stub

}


RosHelper::~RosHelper() {
	// TODO Auto-generated destructor stub
}

void RosHelper::setupRos(TIM_HandleTypeDef *main_htim,  TIM_HandleTypeDef *main_htim2, TIM_HandleTypeDef *encoder_htim, TIM_HandleTypeDef *encoder_htim2, SetUpHelper *main_settings)
{
	nh.initNode();
	settings = main_settings;

	//===Right wheel===
	wheel.set_pins(GPIO_REN1, PIN_REN1, GPIO_LEN1, PIN_LEN1);
	wheel.set_timers(main_htim, CHANNEL1, CHANNEL_REV1);
	wheel.set_robot_params(settings->MAX_LIN_SPEED, settings->MAX_PWD_ALLOWED);

	//===Left wheel===
	wheel2.set_pins(GPIO_REN2, PIN_REN2, GPIO_LEN2, PIN_LEN2);
	wheel2.set_timers(main_htim2, CHANNEL2, CHANNEL_REV2);
	wheel2.set_robot_params(settings->MAX_LIN_SPEED, settings->MAX_PWD_ALLOWED);

	//===Right encoder===
	encoder.init(encoder_htim);
	encoder.set_robot_params(settings->WHEEL_RADIUS, settings->MAX_LIN_SPEED, settings->RAD_PER_TICK);

	//===Left encoder===
	encoder2.init(encoder_htim2);
	encoder2.set_robot_params(settings->WHEEL_RADIUS, settings->MAX_LIN_SPEED, settings->RAD_PER_TICK);

	cmd_vel.init(&nh, &wheel2, &wheel, settings->CMD_VEL_TOPIC);
	cmd_vel.set_robot_params(settings->WHEEL_SEPARATION, settings->MAX_LIN_SPEED, settings->MAX_ANG_VEL);

	odom.init(&nh, &encoder2, &encoder, settings->ODOM_TOPIC, settings->BASE_FRAME, settings->ODOM_FRAME);
	odom.set_robot_params(settings->WHEEL_RADIUS, settings->WHEEL_SEPARATION);
}

void RosHelper::rosLoop(void)
{
	if( xSemaphoreTake( SocketClient::error_semaphore, portMAX_DELAY) == pdTRUE )
	{
		if (SocketClient::is_connected)
		{
			xSemaphoreGive( SocketClient::error_semaphore );
			odom.publish();
			nh.spinOnce();
		}else {
			xSemaphoreGive( SocketClient::error_semaphore );
		}
	}

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


