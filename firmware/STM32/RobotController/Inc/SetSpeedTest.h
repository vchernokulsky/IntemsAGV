#pragma once

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include <std_msgs/Float32.h>

#define MAX_VALUE 256
#define MIN_VALUE 20
#define MAX_SPEED 1.0

#define CHANNEL1 TIM_CHANNEL_1
#define CHANNEL_REV1 TIM_CHANNEL_2
#define CHANNEL2 TIM_CHANNEL_1
#define CHANNEL_REV2 TIM_CHANNEL_2

#define GPIO_REN1 GPIOA
#define GPIO_LEN1 GPIOB
#define PIN_REN1 GPIO_PIN_4
#define PIN_LEN1 GPIO_PIN_0

#define GPIO_REN2 GPIOA
#define GPIO_LEN2 GPIOB
#define PIN_REN2 GPIO_PIN_8
#define PIN_LEN2 GPIO_PIN_10


void from_min_to_max1(TIM_HandleTypeDef *htim){
	HAL_GPIO_WritePin(GPIO_REN1, PIN_REN1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIO_LEN1, PIN_LEN1, GPIO_PIN_SET);
	HAL_TIM_PWM_Start(htim, CHANNEL1);
	HAL_TIM_PWM_Start(htim, CHANNEL_REV1);
	  for(;;){
		 for(int i = 0; i < 256; i++) {
				__HAL_TIM_SetCompare(htim, CHANNEL1, i);
				HAL_Delay(50);
			}

			for(int i = 255; i >= 0; i--) {
				__HAL_TIM_SetCompare(htim, CHANNEL1, i);
				HAL_Delay(50);
			}
	  }
}

void from_min_to_max2(TIM_HandleTypeDef *htim){
	HAL_GPIO_WritePin(GPIO_REN2, PIN_REN2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIO_LEN2, PIN_LEN2, GPIO_PIN_SET);
	HAL_TIM_PWM_Start(htim, CHANNEL2);
	HAL_TIM_PWM_Start(htim, CHANNEL_REV2);
	  for(;;){
		 for(int i = 0; i < 256; i++) {
				__HAL_TIM_SetCompare(htim, CHANNEL2, i);
				HAL_Delay(50);
			}

			for(int i = 255; i >= 0; i--) {
				__HAL_TIM_SetCompare(htim, CHANNEL2, i);
				HAL_Delay(50);
			}
	  }
}

void set_speed1(TIM_HandleTypeDef *htim, int speed){
	HAL_GPIO_WritePin(GPIO_REN1, PIN_REN1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIO_LEN1, PIN_LEN1, GPIO_PIN_SET);
	HAL_TIM_PWM_Start(htim, CHANNEL1);
	HAL_TIM_PWM_Start(htim, CHANNEL_REV1);
	if (speed > 0){
		__HAL_TIM_SetCompare(htim, CHANNEL1, speed);
		__HAL_TIM_SetCompare(htim, CHANNEL_REV1, 0);
	} else {
		__HAL_TIM_SetCompare(htim, CHANNEL1, 0);
		__HAL_TIM_SetCompare(htim, CHANNEL_REV1, speed * (-1));
	}


//	HAL_GPIO_WritePin(GPIO_REN1, PIN_REN1, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIO_LEN1, PIN_LEN1, GPIO_PIN_SET);
////	HAL_TIM_PWM_Start(htim, CHANNEL1);
////	__HAL_TIM_SetCompare(htim, CHANNEL1, 256);
////	__HAL_TIM_SetCompare(htim, CHANNEL1, 0);
//	HAL_TIM_PWM_Start(htim, CHANNEL_REV1);
//	__HAL_TIM_SetCompare(htim, CHANNEL_REV1, 256);
//	__HAL_TIM_SetCompare(htim, CHANNEL_REV1, 0);
}

void set_speed2(TIM_HandleTypeDef *htim, int speed){
	HAL_GPIO_WritePin(GPIO_REN2, PIN_REN2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIO_LEN2, PIN_LEN2, GPIO_PIN_SET);
	HAL_TIM_PWM_Start(htim, CHANNEL2);
	HAL_TIM_PWM_Start(htim, CHANNEL_REV2);
	if (speed > 0){
		__HAL_TIM_SetCompare(htim, CHANNEL2, speed);
		__HAL_TIM_SetCompare(htim, CHANNEL_REV2, 0);
	} else {
		__HAL_TIM_SetCompare(htim, CHANNEL2, 0);
		__HAL_TIM_SetCompare(htim, CHANNEL_REV2, speed * (-1));
	}
}



