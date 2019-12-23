#pragma once

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include <std_msgs/Float32.h>

#define MAX_VALUE 256
#define MAX_SPEED 3.8


class WheelSubscriber
{
private:
	TIM_HandleTypeDef *htim;
	uint32_t Channel;

	GPIO_TypeDef* GPIO_REN;
	GPIO_TypeDef* GPIO_LEN;
	GPIO_TypeDef* GPIO_LPWM;
	GPIO_TypeDef* GPIO_RPWM;

	uint16_t pin_ren;
	uint16_t pin_len;
	uint16_t pin_rpwm;
	uint16_t pin_lpwm;

	ros::Subscriber<std_msgs::Float32,WheelSubscriber> sub;

public:

	void wheel_callback(const std_msgs::Float32& msg){
		float speed = (msg.data / MAX_SPEED) * MAX_VALUE;
		if(speed > MAX_VALUE){
			speed = MAX_VALUE;
		}

		__HAL_TIM_SetCompare(htim, Channel, (int)speed);
		osDelay(50);
	}
	WheelSubscriber(char topic_in[]):sub(topic_in,&WheelSubscriber::wheel_callback, this){}

	void set_ports(GPIO_TypeDef* ren,GPIO_TypeDef* len, GPIO_TypeDef* rpwm, GPIO_TypeDef* lpwm){
		GPIO_REN = ren;
		GPIO_LEN = len;
		GPIO_LPWM = lpwm;
		GPIO_RPWM = rpwm;
	}

	void set_pins(uint16_t ren,uint16_t len, uint16_t rpwm, uint16_t lpwm){
		pin_ren = ren;
		pin_len = len;
		pin_rpwm = lpwm;
		pin_lpwm = rpwm;
	}

	void subscribe(ros::NodeHandle* nh, TIM_HandleTypeDef *main_htim, uint32_t main_channel){
		htim = main_htim;
		Channel = main_channel;
		HAL_GPIO_WritePin(GPIO_REN, pin_ren, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIO_LEN, pin_len, GPIO_PIN_SET);
		 HAL_TIM_PWM_Start(htim, Channel);
		(*nh).subscribe(sub);
	}

};
