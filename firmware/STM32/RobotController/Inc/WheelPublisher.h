#pragma once

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include <std_msgs/Float32.h>

#define RAD_PER_TICK 0.0174533


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

	unsigned graydecode(unsigned gray)
	{
	 unsigned bin;
	 for (bin = 0; gray; gray >>= 1)
	    bin ^= gray;
	 return bin;
	}


public:

	WheelPublisher(ros::NodeHandle* n, char topic_out[], TIM_HandleTypeDef *htim, UartHelper *main_uart_helper):pub(topic_out,&float_msg){
		nh = n;
		(*nh).advertise(pub);
		uart_helper = main_uart_helper;

		prev_tick = 0;
		cur_tick = 0;
		delta = 0;
		encoder_htim = htim;
		HAL_TIM_Encoder_Start(encoder_htim, TIM_CHANNEL_ALL);
	}



//	void publish(){
//		float_msg.data = ((float)cur_tick - (float)prev_tick) * RAD_PER_TICK;
//		prev_tick = cur_tick;
//		pub.publish(&float_msg);
//	}

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

	void push(uint8_t gray_code){
		uint8_t code = gray_code;
//		xQueueSend( q, ( void * ) &code, portMAX_DELAY  );
	}



	void calculate_ang(uint8_t gray_code){
			unsigned code = graydecode(gray_code);
//			if (code == 0)
//			{
//			   if (previous_code == 3){
//	//		     Serial.println("->");
//				 cur_tick += 1;
//			   }
//			   else if (previous_code == 1)
//	//		     Serial.println("<-");
//				 cur_tick -= 1;
//			 }
//			 previous_code = code;
		}



};
