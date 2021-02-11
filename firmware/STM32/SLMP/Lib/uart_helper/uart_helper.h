#pragma once

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"

class UartHelper {
private:
	xQueueHandle uart_queue;
	UART_HandleTypeDef *huart;
public:
	UartHelper();
	virtual ~UartHelper();
	void init(UART_HandleTypeDef *main_huart);
	void printf(const char* fmt, ...);
	void printf(const uint8_t* buff);
	void UARTTask();
};
