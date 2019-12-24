#include <mainpp.h>
#include <string>
#include "RosHelper.h"


UART_HandleTypeDef *huart;
//SPI_HandleTypeDef *hspi;
//UartHelper uart_helper;
//SocketClient socket_client;
RosHelper* ros_helper = nullptr;

uint32_t countA = 0;
uint32_t countB = 0;

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	if (ros_helper != nullptr){
		ros_helper->flush();
	}
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (ros_helper != nullptr){
		ros_helper->reset_buf();
	}
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (ros_helper != nullptr){
		ros_helper->exti_Callback(GPIO_Pin);
	}
}

//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//  if (GPIO_Pin == GPIO_PIN_4)
//  {
//	  countA++;
//	  return;
//  }
//  if (GPIO_Pin == GPIO_PIN_10)
//   {
// 	  countB++;
// 	  return;
//   }
//  else
//    {
//      __NOP();
//
//    }
//}




void StartRosTask(void const * argument){
	ros_helper->RosTask();
}

void StartSetSpeedTask(void const * argument){
	ros_helper->setSpeedTask();
}

void StartSetSpeedTask2(void const * argument){
	ros_helper->setSpeedTask2();
}

//void StartReadSpeedTask(void const * argument){
//	ros_helper->readSpeedTask();
//}
//
//void StartReadSpeedTask2(void const * argument){
//	ros_helper->readSpeedTask2();
//}






//void StartSecondTask(void const * argument)
//{
//  for(;;)
//  {
//	  uart_helper.printf("!!!!!!!!!!!!!!!!\n\0");
//	  osDelay(500);
//  }
//}


//void StartUARTTask(void const * argument)
//{
//	uart_helper.UARTTask();
//}

//void StartSocketSendTask(void const * argument){
//	std::string str = "1234";
//	 const static uint16_t rbuflen = 128;
//	uint8_t rbuf[rbuflen];
//	uint16_t Size = 5;
//	uint32_t* rdmaInd;
//	  for(;;)
//	  {
//		  socket_client.socket_receive(rbuf, Size, rdmaInd);
//		  uart_helper.printf(rbuf);
//		  socket_client.socket_send(str.c_str(), str.length());
//		  osDelay(100);
//	  }
////	vTaskDelete( NULL );
//}

//void setup(UART_HandleTypeDef *main_huart, SPI_HandleTypeDef *main_hspi1){
////	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
//	  huart = main_huart;
//	  hspi = main_hspi1;
//	  uart_helper.init(huart);
//	  socket_client.init(hspi, &uart_helper);
//	  socket_client.socket_connect();
//	  ros_helper.setupRos(&uart_helper);
//
////	  osThreadDef(SecondTask, StartSecondTask, osPriorityNormal, 1, 256);
////	  osThreadCreate(osThread(SecondTask), NULL);
//
//	  osThreadDef(UartTask, StartUARTTask, osPriorityNormal, 1, 256);
//	  osThreadCreate(osThread(UartTask), NULL);
//
////	  osThreadDef(SocketSendTask, StartSocketSendTask, osPriorityNormal, 1, 256);
////	  osThreadCreate(osThread(SocketSendTask), NULL);
//
//	  osThreadDef(RosTask, StartRosTask, osPriorityNormal, 1, 256);
//	  osThreadCreate(osThread(RosTask), NULL);
//
//
//}

void setup(UART_HandleTypeDef *main_huart, TIM_HandleTypeDef *main_htim, TIM_HandleTypeDef *main_htim2){
	  huart = main_huart;
	  ros_helper = new RosHelper();
	  ros_helper->setupRos(main_htim, main_htim2);

	  /*===============ROS=======================================*/
	  osThreadDef(RosTask, StartRosTask, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(RosTask), NULL);

	  osThreadDef(setSpeedTask, StartSetSpeedTask, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(setSpeedTask), NULL);

	  osThreadDef(setSpeedTask2, StartSetSpeedTask2, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(setSpeedTask2), NULL);

//	  osThreadDef(readSpeedTask, StartReadSpeedTask, osPriorityNormal, 1, 256);
//	  osThreadCreate(osThread(readSpeedTask), NULL);
//
//	  osThreadDef(readSpeedTask2, StartReadSpeedTask2, osPriorityNormal, 1, 256);
//	  osThreadCreate(osThread(readSpeedTask2), NULL);
}

void loop(void){
	ros_helper->rosLoop();
}


