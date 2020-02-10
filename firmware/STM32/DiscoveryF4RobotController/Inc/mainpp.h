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


void memory_setup();
void threds_setup(TIM_HandleTypeDef *main_htim,  TIM_HandleTypeDef *main_htim2, TIM_HandleTypeDef *encoder_htim, TIM_HandleTypeDef *encoder_htim2);

#ifdef __cplusplus
}
#endif


#endif /* MAINPP_H_ */
