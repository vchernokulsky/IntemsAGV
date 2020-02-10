/*
 * UartHelper.cpp
 *
 *  Created on: Nov 27, 2019
 *      Author: developer
 */


#include "UartHelper.h"

UartHelper::UartHelper() {
	// TODO Auto-generated constructor stub

}
UartHelper::UartHelper(UART_HandleTypeDef *main_huart) {
	// TODO Auto-generated constructor stub
	uart_queue = xQueueCreate( UART_QUEUE_LEN, sizeof( get_buff ) );
	huart = main_huart;
}

UartHelper::~UartHelper() {
	// TODO Auto-generated destructor stub
}

void UartHelper::init(UART_HandleTypeDef *main_huart){
	uart_queue = xQueueCreate( UART_QUEUE_LEN, sizeof( get_buff ) );
	huart = main_huart;
}

void UartHelper::printf(const char* fmt, ...) {
	char buff[UART_BUF_SIZE];
	va_list args;
	va_start(args, fmt);
	vsnprintf((char*)buff, sizeof(buff), fmt, args);
	xQueueSend( uart_queue, ( void * ) &buff, portMAX_DELAY  );
	va_end(args);
}
void UartHelper::printf(const uint8_t* buff) {
    xQueueSend( uart_queue, ( void * ) &buff, portMAX_DELAY  );
}

void UartHelper::UARTTask()
{
  for(;;)
  {
	  char buff[UART_BUF_SIZE] = {};
	  xQueueReceive( uart_queue,  buff , portMAX_DELAY );
	  int len = strlen(buff);
	  HAL_UART_Transmit(huart, (uint8_t*)buff, len, HAL_MAX_DELAY);
	  osDelay(UART_TRANSMIT_DELAY);
  }
}


