#include <mainpp.h>
#include <string>
#include "SocketClient.h"

UART_HandleTypeDef *huart;
SPI_HandleTypeDef *hspi;
UartHelper uart_helper;
SocketClient socket_client;


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

void StartSocketSendTask(void const * argument)
{
	std::string str = "1234";
	 const static uint16_t rbuflen = 128;
	uint8_t rbuf[rbuflen];
	uint16_t Size = 5;
	uint32_t* rdmaInd;
	  for(;;)
	  {
		  socket_client.socket_receive(rbuf, Size, rdmaInd);
		  uart_helper.printf(rbuf);
		  socket_client.socket_send(str.c_str(), str.length());
		  osDelay(100);
	  }
//	vTaskDelete( NULL );
}

void setup(UART_HandleTypeDef *main_huart, SPI_HandleTypeDef *main_hspi1,
		TIM_HandleTypeDef *main_htim, TIM_HandleTypeDef *main_htim2,
		TIM_HandleTypeDef *main_encoder_htim1, TIM_HandleTypeDef *main_encoder_htim2)
{

	  huart = main_huart;
	  hspi = main_hspi1;
	  uart_helper.init(huart);

	  socket_client.init(hspi, &uart_helper);
	  HAL_Delay(1000 * 2);
	  socket_client.socket_connect();


	  //****** UART **********
	  osThreadDef(UartTask, StartUARTTask, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(UartTask), NULL);
//	  //****** Socket **********
//	  osThreadDef(SocketTask, StartSocketTask, osPriorityNormal, 1, 256);
//	  osThreadCreate(osThread(SocketTask), NULL);

	  //========== ROS ===============
//

	  //****** UART Test ***************
	  osThreadDef(SecondTask, StartSecondTask, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(SecondTask), NULL);

	  //******* Socket Test *************
	  osThreadDef(SocketSendTask, StartSocketSendTask, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(SocketSendTask), NULL);

}

