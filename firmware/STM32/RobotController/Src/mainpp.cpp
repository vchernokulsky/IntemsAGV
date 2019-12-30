#include <mainpp.h>
#include <string>
#include "UartHelper.h"
#include "SocketClient.h"
#include "RosHelper.h"


UART_HandleTypeDef *huart;
SPI_HandleTypeDef *hspi;
UartHelper uart_helper;
SocketClient socket_client;
RosHelper ros_helper;




void StartRosTask(void const * argument){
	ros_helper.RosTask();
}


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

void StartSocketSendTask(void const * argument){
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

void setup(UART_HandleTypeDef *main_huart, SPI_HandleTypeDef *main_hspi1){
//	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
	  huart = main_huart;
	  hspi = main_hspi1;
	  uart_helper.init(huart);
	  socket_client.init(hspi, &uart_helper);
	  socket_client.socket_connect();
	  ros_helper.setupRos(&uart_helper);

//	  osThreadDef(SecondTask, StartSecondTask, osPriorityNormal, 1, 256);
//	  osThreadCreate(osThread(SecondTask), NULL);

	  osThreadDef(UartTask, StartUARTTask, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(UartTask), NULL);

//	  osThreadDef(SocketSendTask, StartSocketSendTask, osPriorityNormal, 1, 256);
//	  osThreadCreate(osThread(SocketSendTask), NULL);

	  osThreadDef(RosTask, StartRosTask, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(RosTask), NULL);


}

