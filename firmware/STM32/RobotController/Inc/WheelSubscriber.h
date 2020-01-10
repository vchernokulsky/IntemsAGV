#pragma once

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include <std_msgs/Float32.h>

#define MAX_VALUE 256
#define MIN_VALUE 20
#define MAX_SPEED 1.0


class WheelSubscriber
{
private:
	xQueueHandle q;

	TIM_HandleTypeDef *htim;
	uint32_t Channel;
	uint32_t Channel_rev;

	ros::Subscriber<std_msgs::Float32,WheelSubscriber> sub;

public:

	void wheel_callback(const std_msgs::Float32& msg){
		float data = msg.data;
		xQueueSend( q, ( void * ) &data, portMAX_DELAY  );

	}

	void set_speed(void){
		float data;
		xQueueReceive( q, &( data ), portMAX_DELAY );
		if(data == 0){
			__HAL_TIM_SetCompare(htim, Channel, 0);
			__HAL_TIM_SetCompare(htim, Channel_rev, 0);
			return;
		}
		int sign = 1;
		if(data < 0){
			sign = -1;
		}
		float speed = (sign * data / MAX_SPEED ) * (MAX_VALUE - MIN_VALUE) + MIN_VALUE;
		if(speed > MAX_VALUE){
			speed = MAX_VALUE;
		}
		if(sign > 0){
			__HAL_TIM_SetCompare(htim, Channel, (int)speed);
			__HAL_TIM_SetCompare(htim, Channel_rev, 0);
		} else {
			__HAL_TIM_SetCompare(htim, Channel, 0);
			__HAL_TIM_SetCompare(htim, Channel_rev, (int)speed);
		}
	}



	WheelSubscriber(char topic_in[]):sub(topic_in,&WheelSubscriber::wheel_callback, this){}


	void set_pins(GPIO_TypeDef* GPIO_REN, uint16_t pin_ren,GPIO_TypeDef* GPIO_LEN, uint16_t pin_len){
		HAL_GPIO_WritePin(GPIO_REN, pin_ren, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIO_LEN, pin_len, GPIO_PIN_SET);
	}

	void set_timers(TIM_HandleTypeDef *main_htim, uint32_t main_channel, uint32_t main_channel_rev){
		htim = main_htim;
		Channel = main_channel;
		Channel_rev = main_channel_rev;
		q = xQueueCreate( 8, sizeof( float ) );
	}

	void subscribe(ros::NodeHandle* nh){
		HAL_TIM_PWM_Start(htim, Channel);
		HAL_TIM_PWM_Start(htim, Channel_rev);
		(*nh).subscribe(sub);
	}

};
