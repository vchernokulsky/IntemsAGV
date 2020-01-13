#pragma once

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include <std_msgs/Float32.h>

#define RAD_PER_TICK 0.0174533
#define RADIUS 0.2
#define MAX_SPEED 3.8


class WheelPublisher
{
private:
	ros::NodeHandle* nh;
	TIM_HandleTypeDef *encoder_htim = nullptr;
	UartHelper *uart_helper = nullptr;
	std_msgs::Float32 float_msg;
	ros::Publisher pub;

	uint16_t prev_tick;
	uint16_t cur_tick;
	uint16_t delta;
	uint8_t encoderDirection;

	uint16_t speed_prev_tick;
	uint16_t speed_cur_tick;
	uint32_t prev_time;
	uint32_t cur_time;
	uint32_t delta_time;
	uint16_t speed_tick_delta;
	uint8_t speed_encoder_direction;

	float cur_speed;

public:

	WheelPublisher(ros::NodeHandle* n, char topic_out[], TIM_HandleTypeDef *htim, UartHelper *main_uart_helper):pub(topic_out,&float_msg){
		nh = n;
		(*nh).advertise(pub);
		uart_helper = main_uart_helper;

		prev_tick = 0;
		cur_tick = 0;
		delta = 0;

		speed_prev_tick = 0;
		speed_cur_tick = 0;
		prev_time = 0;
		cur_time = 0;
		delta_time = 0;
		speed_tick_delta = 0;
		speed_encoder_direction = 0;

		encoder_htim = htim;
		HAL_TIM_Encoder_Start(encoder_htim, TIM_CHANNEL_ALL);
	}



	void publish(){
		cur_tick = __HAL_TIM_GET_COUNTER(encoder_htim);
//		uart_helper->printf("\r\nCount=%i\r\n", cur_tick);
		encoderDirection = __HAL_TIM_IS_TIM_COUNTING_DOWN(encoder_htim);
		if(encoderDirection == 0){
			delta = cur_tick - prev_tick;
			float_msg.data = delta * RAD_PER_TICK / 4;
		} else {
			delta = prev_tick - cur_tick;
			float_msg.data = (-1) * delta * RAD_PER_TICK / 4;
		}


//		float_msg.data = delta ;
		prev_tick = cur_tick;
		pub.publish(&float_msg);
	}

	float get_speed(){
		speed_cur_tick = __HAL_TIM_GET_COUNTER(encoder_htim);
		cur_time = HAL_GetTick();
		delta_time = cur_time - prev_time;
		speed_encoder_direction = __HAL_TIM_IS_TIM_COUNTING_DOWN(encoder_htim);
		if(speed_encoder_direction == 0){
			speed_tick_delta = speed_cur_tick - speed_prev_tick;
			cur_speed = (speed_tick_delta / 4) * (RAD_PER_TICK * RADIUS)  /  ((float)delta_time / 1000.0);
		} else {
			speed_tick_delta = speed_prev_tick - speed_cur_tick;
			cur_speed = (-1) * (speed_tick_delta / 4) * (RAD_PER_TICK * RADIUS)  /  ((float)delta_time / 1000.0);
		}


//		float_msg.data = delta ;
		speed_prev_tick = speed_cur_tick;
		prev_time = cur_time;
		uart_helper->printf("\r\nCUR_SPEED=%i\r\n", cur_speed);
		return cur_speed / MAX_SPEED;
	}










};
