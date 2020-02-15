#ifndef MAINPP_H_
#define MAINPP_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "cmsis_os.h"

#include "stm32f4xx_hal.h"

 void setup(UART_HandleTypeDef *main_huart, SPI_HandleTypeDef *main_hspi1,
		 TIM_HandleTypeDef *main_htim, TIM_HandleTypeDef *main_htim2,
		 TIM_HandleTypeDef *main_encoder_htim1, TIM_HandleTypeDef *main_encoder_htim2);

 void prepareSLMPRequest(void);

#ifdef __cplusplus
}
#endif


#endif /* MAINPP_H_ */
