/*
 * UartHelper.cpp
 *
 *  Created on: Nov 27, 2019
 *      Author: developer
 */

#include <stdlib.h>
#include "UartHelper.h"

UartHelper::UartHelper() {
	// TODO Auto-generated constructor stub
	char buff[20];
	uart_queue = xQueueCreate( 8, sizeof( buff ) );
}

UartHelper::~UartHelper() {
	// TODO Auto-generated destructor stub
}

void UartHelper::init(UART_HandleTypeDef *main_huart){
	huart = main_huart;
}

void UartHelper::printf(const char* fmt, ...) {
	char buff[20];
    //int len = strlen(fmt);
    //void* buff = calloc(len+1, sizeof(char));
    va_list args;
    va_start(args, fmt);
    vsnprintf((char*)buff, sizeof(buff), fmt, args);
    xQueueSend( uart_queue, ( void * ) &buff, portMAX_DELAY  );
    va_end(args);
}
void UartHelper::printf(const uint8_t* buff) {
//	uint8_t buff[20];
//    va_list args;
//    va_start(args, fmt);
//    vsnprintf(buff, sizeof(buff), fmt, args);
    xQueueSend( uart_queue, ( void * ) &buff, portMAX_DELAY  );
//    va_end(args);
}

void UartHelper::UARTTask()
{
  for(;;)
  {
	  char buff[20];
	  xQueueReceive( uart_queue, &( buff ), portMAX_DELAY );
	  int len = strlen(buff);
	  HAL_UART_Transmit(huart, (uint8_t*)buff, len, HAL_MAX_DELAY);
	  osDelay(50);
  }
}


