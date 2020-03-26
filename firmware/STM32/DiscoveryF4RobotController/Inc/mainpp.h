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
void external_memory_init(I2C_HandleTypeDef *main_hi2c1);
void threds_setup(TIM_HandleTypeDef *main_htim,  TIM_HandleTypeDef *main_htim2, TIM_HandleTypeDef *encoder_htim, TIM_HandleTypeDef *encoder_htim2);
void set_default();

uint8_t* get_local_ip_ptr();
uint8_t* get_network_mask_ptr();
uint8_t* get_gateaway_ptr();

#ifdef __cplusplus
}
#endif


#endif /* MAINPP_H_ */
