#pragma once

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include <std_msgs/Float32.h>

class WheelPublisher
{
private:
	float wheel_radius;
	float max_lin_speed;
	float rad_per_tick;
	SemaphoreHandle_t semaphore;
	TIM_HandleTypeDef *encoder_htim = nullptr;

	uint16_t prev_tick;
	uint16_t cur_tick;
	uint16_t delta_tick;

	uint32_t prev_time;
	uint32_t cur_time;
	uint32_t delta_time;
	uint32_t prev_distance_time;

	float cur_speed; // Speed in m/s
	float tick_per_sek;
	int64_t distance_tick;

public:
	WheelPublisher(){
	}

	void init(TIM_HandleTypeDef *htim){
		WheelPublisher::semaphore = xSemaphoreCreateMutex();

		prev_tick = 0;
		delta_tick = 0;

		prev_time = 0;
		cur_time = 0;
		delta_time = 0;
		prev_distance_time = 0;

		cur_speed = 0.0;
		distance_tick = 0;

		encoder_htim = htim;
		HAL_TIM_Encoder_Start(encoder_htim, TIM_CHANNEL_ALL);
		cur_tick = __HAL_TIM_GET_COUNTER(encoder_htim);
		prev_tick = cur_tick;
	}

	void set_robot_params(float radius, float lin_speed, float rad){
		wheel_radius = radius;
		max_lin_speed = lin_speed;
		rad_per_tick = rad;
	}

	void tick_calculate(){
			cur_tick = __HAL_TIM_GET_COUNTER(encoder_htim);
			cur_time = HAL_GetTick();
			delta_time = cur_time - prev_time;
			if(__HAL_TIM_IS_TIM_COUNTING_DOWN(encoder_htim) == 0){
				delta_tick = cur_tick - prev_tick;
				distance_tick += delta_tick;
				tick_per_sek = ((float)delta_tick / 4.0)  /  ((float)delta_time / 1000.0);
				if( xSemaphoreTake( semaphore, portMAX_DELAY) == pdTRUE )
					{
					cur_speed = ((float)delta_tick / 4.0) * (rad_per_tick * wheel_radius)  /  ((float)delta_time / 1000.0);
						xSemaphoreGive( semaphore );
					}

			} else {
				delta_tick = prev_tick - cur_tick;
				distance_tick -= delta_tick;
				tick_per_sek = (-1) * ((float)delta_tick / 4.0)   /  ((float)delta_time / 1000.0);
				if( xSemaphoreTake( semaphore, portMAX_DELAY) == pdTRUE )
					{
					cur_speed = (-1) * ((float)delta_tick / 4.0) * (rad_per_tick * wheel_radius)  /  ((float)delta_time / 1000.0);
						xSemaphoreGive( semaphore );
					}

			}
			prev_tick = cur_tick;
			prev_time = cur_time;
	}


	int16_t get_speed(){
		// speed in %
//		return cur_speed / MAX_LIN_SPEED;
		int16_t ret = 0;
		if( xSemaphoreTake( semaphore, portMAX_DELAY) == pdTRUE )
		{
			ret = std::round(cur_speed / max_lin_speed * MAX_PWD);
			xSemaphoreGive( semaphore );
		}
		return ret;
//		return cur_speed;
	}

	float get_tick_per_sek(){
		// speed in %
		return tick_per_sek;
	}

	float get_distance(){
		// distance in rad
		int32_t ret_dist = distance_tick;
		distance_tick = 0;
		return (float)ret_dist * rad_per_tick / 4.0;
	}

	double  get_distance_time(){
		int16_t ret_time = cur_time - prev_distance_time;
		prev_distance_time = cur_time;
		return (double)ret_time;
	}

};
