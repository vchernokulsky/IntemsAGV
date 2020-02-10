#pragma once

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include <std_msgs/Float32.h>
#include "User_config.h"

class WheelPublisher
{
private:
	TIM_HandleTypeDef *encoder_htim = nullptr;
	UartHelper *uart_helper = nullptr;

	uint16_t prev_tick;
	uint16_t cur_tick;
	uint16_t delta_tick;

	uint32_t prev_time;
	uint32_t cur_time;
	uint32_t delta_time;
	uint32_t prev_distance_time;

	float cur_speed; // Speed in m/s
	float tick_per_sek;
	int32_t distance_tick;

public:
	WheelPublisher(){
	}

	WheelPublisher(TIM_HandleTypeDef *htim, UartHelper *main_uart_helper){
		uart_helper = main_uart_helper;

		prev_tick = 0;
		cur_tick = 0;
		delta_tick = 0;

		prev_time = 0;
		cur_time = 0;
		delta_time = 0;
		prev_distance_time = 0;

		cur_speed = 0.0;
		distance_tick = 0;

		encoder_htim = htim;
		HAL_TIM_Encoder_Start(encoder_htim, TIM_CHANNEL_ALL);
	}

	void init(TIM_HandleTypeDef *htim, UartHelper *main_uart_helper){
		uart_helper = main_uart_helper;

		prev_tick = 0;
		cur_tick = 0;
		delta_tick = 0;

		prev_time = 0;
		cur_time = 0;
		delta_time = 0;
		prev_distance_time = 0;

		cur_speed = 0.0;
		distance_tick = 0;

		encoder_htim = htim;
		HAL_TIM_Encoder_Start(encoder_htim, TIM_CHANNEL_ALL);
	}

	void tick_calculate(){
			cur_tick = __HAL_TIM_GET_COUNTER(encoder_htim);
			cur_time = HAL_GetTick();
			delta_time = cur_time - prev_time;
			if(__HAL_TIM_IS_TIM_COUNTING_DOWN(encoder_htim) == 0){
				delta_tick = cur_tick - prev_tick;
				distance_tick += delta_tick;
				tick_per_sek = ((float)delta_tick / 4.0)  /  ((float)delta_time / 1000.0);
				cur_speed = ((float)delta_tick / 4.0) * (RAD_PER_TICK * RADIUS)  /  ((float)delta_time / 1000.0);
			} else {
				delta_tick = prev_tick - cur_tick;
				distance_tick -= delta_tick;
				tick_per_sek = (-1) * ((float)delta_tick / 4.0)   /  ((float)delta_time / 1000.0);
				cur_speed = (-1) * ((float)delta_tick / 4.0) * (RAD_PER_TICK * RADIUS)  /  ((float)delta_time / 1000.0);
			}
			prev_tick = cur_tick;
			prev_time = cur_time;
	}


	float get_speed(){
		// speed in %
		return cur_speed / MAX_LIN_SPEED;
	}

	float get_tick_per_sek(){
		// speed in %
		return tick_per_sek;
	}

	float get_distance(){
		// distance in rad
		int32_t ret_dist = distance_tick;
		distance_tick = 0;
		return (float)ret_dist * RAD_PER_TICK / 4.0;
	}

	double  get_distance_time(){
		int16_t ret_time = cur_time - prev_distance_time;
		prev_distance_time = cur_time;
		return (double)ret_time;
	}

};
