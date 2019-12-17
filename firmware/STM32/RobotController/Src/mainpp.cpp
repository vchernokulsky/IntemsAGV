#include <mainpp.h>
#include "UartHelper.h"


UART_HandleTypeDef *huart;
SPI_HandleTypeDef *hspi;
UartHelper uart_helper;


void StartSecondTask(void const * argument)
{
  for(;;)
  {
	  uart_helper.printf("!!!!!!!!!!!!!!!!\n\0");
	  osDelay(500);
  }
}


void StartUARTTask(void const * argument)
{
	uart_helper.UARTTask();
}

void setup(UART_HandleTypeDef *main_huart, SPI_HandleTypeDef *main_hspi1){
	  huart = main_huart;
	  hspi = main_hspi1;
	  uart_helper.init(huart);


	  osThreadDef(SecondTask, StartSecondTask, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(SecondTask), NULL);

	  osThreadDef(UartTask, StartUARTTask, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(UartTask), NULL);


}

