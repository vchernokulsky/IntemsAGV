/*
 * UartHelper.h
 *
 *  Created on: Nov 27, 2019
 *      Author: developer
 */

#ifndef UARTHELPER_H_
#define UARTHELPER_H_

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"

#include "User_config.h"
#include "System_config.h"

class UartHelper {
private:
	xQueueHandle uart_queue;
	UART_HandleTypeDef *huart;

	char put_buff[UART_BUF_SIZE];
	char get_buff[UART_BUF_SIZE];
public:
	UartHelper();
	UartHelper(UART_HandleTypeDef *main_huart);
	virtual ~UartHelper();
	void init(UART_HandleTypeDef *main_huart);
	void printf(const char* fmt, ...);
	void printf(const uint8_t* buff);
	void UARTTask();
};

#endif /* UARTHELPER_H_ */
