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

//main_htim - timer for RPWM1/LPWM1
//main_htim2 - timer for RPWM2/LPWM2
//main_encoder_htim1 - timer for encoder1
//main_encoder_htim2 - timer for encoder2

void memory_setup();
void threds_setup();
void StartSocetClientTask(void *arg);

#ifdef __cplusplus
}
#endif


#endif /* MAINPP_H_ */
