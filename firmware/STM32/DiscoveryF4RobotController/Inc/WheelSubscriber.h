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

	int16_t target_vel;
	int16_t cur_pwd;

	double err_prev;
	double err_cur;

	double impact;
	double integral_part;

	double Kp;
	double Ki;
	double Kd;



public:

	void wheel_callback(float data){
		target_vel = std::round(data / MAX_LIN_SPEED * MAX_PWD);
		fit_limits(&target_vel);
	}

	void set_speed(int16_t cur_vel){
		uint8_t speed = 0;
		uint8_t speed_rev = 0;

		err_cur = target_vel - cur_vel;

		/***** P step regulator ******/
		impact = 1;
		if (err_cur < 0){
			err_cur = -err_cur;
			impact = -1;
		}
		if (err_cur > SPEED_DELTA_L)
		{
			impact *= PWD_STEP_L;
		} else
		{
			if (err_cur > SPEED_DELTA_M)
			{
			   impact *= PWD_STEP_M;
			} else
			{
				if (err_cur > SPEED_DELTA_S)
				{
				   impact *= PWD_STEP_S;
				} else
				{
					impact *= err_cur;
				}
			}
	     }
		/***************************/

	   /****** PID regulator ******/
//		impact =  Kp*err_cur;
//		integral_part = integral_part + Ki * err_cur;
//		impact += integral_part;
//		impact += Kd * (err_cur - err_prev);
		/**************************/

		cur_pwd += std::round(impact);
		fit_limits(&cur_pwd);

		if(cur_pwd > 0){
			speed = cur_pwd;
		}
		if(cur_pwd < 0){
			speed_rev = (-1) * cur_pwd;
		}

		__HAL_TIM_SetCompare(htim, Channel, speed);
		__HAL_TIM_SetCompare(htim, Channel_rev, speed_rev);

	}


	WheelSubscriber(){
		target_vel = 0;
		cur_pwd = 0;

		err_cur = 0;
		err_prev = 0;
		integral_part = 0;

		Kp = KP_DEFAULT;
		Ki = KI_DEAFAULT;
		Kd = KD_DEFAULT;
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

	void fit_limits(int16_t *val)
	{
		if(*val > MAX_PWD_ALLOWED){
			*val = MAX_PWD_ALLOWED;
		}
		if(*val < -MAX_PWD_ALLOWED){
			*val = -MAX_PWD_ALLOWED;
		}
	}

	void set_pid(double p, double i, double d)
	{
		Kp = p;
		Ki = i;
		Kd = d;
	}

};
