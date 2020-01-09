#include <mainpp.h>
#include <string>
#include "UartHelper.h"
#include "SocketClient.h"
#include "RosHelper.h"

#include "SetSpeedTest.h"


UART_HandleTypeDef *huart;
SPI_HandleTypeDef *hspi;
UartHelper uart_helper;
SocketClient socket_client;
RosHelper* ros_helper  = nullptr;

TIM_HandleTypeDef *htim = nullptr;
TIM_HandleTypeDef *htim2 = nullptr;




void StartRosTask(void const * argument){
	ros_helper->RosTask();
}
void StartSetSpeedTask(void const * argument){
	ros_helper->setSpeedTask();
}
void StartSetSpeedTask2(void const * argument){
	ros_helper->setSpeedTask2();
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

void StartWheelSpeedTask(void const * argument){
	set_speed1(htim, 128);
	set_speed2(htim2, 128);
	for (;;){
		osDelay(100);
	}
}

void setup(UART_HandleTypeDef *main_huart, SPI_HandleTypeDef *main_hspi1, TIM_HandleTypeDef *main_htim, TIM_HandleTypeDef *main_htim2){
//	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
	  huart = main_huart;
	  hspi = main_hspi1;
	  uart_helper.init(huart);

	  socket_client.init(hspi, &uart_helper);
	  socket_client.socket_connect();


	  //****** UART **********
	  osThreadDef(UartTask, StartUARTTask, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(UartTask), NULL);

	  //========== ROS ===============
//
//	  //******** SpinOnce ***********
//	  ros_helper = new RosHelper();
//	  ros_helper->setupRos(&uart_helper, main_htim, main_htim2);
//	  osThreadDef(RosTask, StartRosTask, osPriorityNormal, 1, 256);
//	  osThreadCreate(osThread(RosTask), NULL);
//
//	  //**** Wheel1 subscriber ********
//	  osThreadDef(setSpeedTask, StartSetSpeedTask, osPriorityNormal, 1, 256);
//	  osThreadCreate(osThread(setSpeedTask), NULL);
//
//	  //**** Wheel2 subscriber ********
//	  osThreadDef(setSpeedTask2, StartSetSpeedTask2, osPriorityNormal, 1, 256);
//	  osThreadCreate(osThread(setSpeedTask2), NULL);

	  //==============================

	  //****** UART Test ***************
//	  osThreadDef(SecondTask, StartSecondTask, osPriorityNormal, 1, 256);
//	  osThreadCreate(osThread(SecondTask), NULL);

	  //******* Socket Test *************
//	  osThreadDef(SocketSendTask, StartSocketSendTask, osPriorityNormal, 1, 256);
//	  osThreadCreate(osThread(SocketSendTask), NULL);

	  //******* Wheel Speed Test ***********
	  htim = main_htim;
	  htim2 = main_htim2;
	  osThreadDef(wheelSpeedTask, StartWheelSpeedTask, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(wheelSpeedTask), NULL);





}

