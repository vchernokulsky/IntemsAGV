#pragma once

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include <std_msgs/Float32.h>

#include "User_config.h"

class WheelSubscriber
{
private:

	TIM_HandleTypeDef *htim;
	uint32_t Channel;
	uint32_t Channel_rev;

	float target_vel;
	int16_t cur_pwd;

public:

	void wheel_callback(float data){
		target_vel = data;
	}

	void set_speed(float cur_vel){
		uint8_t speed = 0;
		uint8_t speed_rev = 0;

		float delta_abs = target_vel - cur_vel;
		int vel_sign = 1;
		if (delta_abs < 0){
			delta_abs = -delta_abs;
			vel_sign = -1;
		}
		int16_t pwd_delta = 0;
		if (delta_abs > SPEED_DELTA_L){
			pwd_delta = PWD_STEP_L;
		} else {
			if (delta_abs > SPEED_DELTA_M){
				pwd_delta = PWD_STEP_M;
			} else {
				if (delta_abs > SPEED_DELTA_S){
					pwd_delta = PWD_STEP_S;
				}
			}

		}
		if (pwd_delta == 0 && target_vel==0){
			cur_pwd = 0;
		} else {
			cur_pwd += vel_sign*pwd_delta;
		}

		if(cur_pwd > 0){
			if (cur_pwd > MAX_PWD_ALLOWED){
				cur_pwd = MAX_PWD_ALLOWED;
			}
			speed = cur_pwd;

		}
		if(cur_pwd < 0){
			if (cur_pwd < -MAX_PWD_ALLOWED){
				cur_pwd = -MAX_PWD_ALLOWED;
			}
			speed_rev = (-1) * cur_pwd;
		}
		__HAL_TIM_SetCompare(htim, Channel, speed);
		__HAL_TIM_SetCompare(htim, Channel_rev, speed_rev);


	}


	WheelSubscriber(){
		target_vel = 0;
		cur_pwd = 0;
	}

	void set_pins(GPIO_TypeDef* gpio_ren, uint16_t pin_ren,GPIO_TypeDef* gpio_len, uint16_t pin_len){
		HAL_GPIO_WritePin(gpio_ren, pin_ren, GPIO_PIN_SET);
		HAL_GPIO_WritePin(gpio_len, pin_len, GPIO_PIN_SET);
	}

	void set_timers(TIM_HandleTypeDef *main_htim, uint32_t main_channel, uint32_t main_channel_rev){
		htim = main_htim;
		Channel = main_channel;
		Channel_rev = main_channel_rev;
		HAL_TIM_PWM_Start(htim, Channel);
		HAL_TIM_PWM_Start(htim, Channel_rev);
	}

};
