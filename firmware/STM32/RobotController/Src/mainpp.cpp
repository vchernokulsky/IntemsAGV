#include <mainpp.h>
#include <string>
#include "UartHelper.h"
#include "SocketClient.h"
#include "RosHelper.h"

#include "SetSpeedTest.h"


UART_HandleTypeDef *huart;
SPI_HandleTypeDef *hspi;
UartHelper uart_helper;
SocketClient *socket_client;
RosHelper* ros_helper  = nullptr;

static TIM_HandleTypeDef *htim = nullptr;
static TIM_HandleTypeDef *htim2 = nullptr;

static TIM_HandleTypeDef *encoder_htim = nullptr;

uint64_t encoderCount;
uint8_t encoderDirection;



void StartRosTask(void const * argument){
	ros_helper->RosTask();
}
void StartSetSpeedTask(void const * argument){
	ros_helper->setSpeedTask();
}
void StartSetSpeedTask2(void const * argument){
	ros_helper->setSpeedTask2();
}

void StartEncoderTask(void const * argument){
	ros_helper->encoderTask();
}

void StartEncoderTask2(void const * argument){
	ros_helper->encoderTask2();
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
void StartSocketStateTask(void const * argument)
{
	socket_client->SocketStateTask();
}
void StartCheckFreezingTask(void const * argument)
{
	socket_client->CheckFreezingTask();
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
		  socket_client->socket_receive(rbuf, Size, rdmaInd);
		  uart_helper.printf(rbuf);
		  socket_client->socket_send(str.c_str(), str.length());
		  osDelay(100);
	  }
//	vTaskDelete( NULL );
}

void StartWheelSpeedTask(void const * argument){
	set_speed1(htim, 40);
	set_speed2(htim2, 40);
	for (;;){
		osDelay(100);
	}
}

void StartEncoderTestTask(void const * argument){
	HAL_TIM_Encoder_Start(encoder_htim, TIM_CHANNEL_ALL);
	for(;;){
		encoderCount = __HAL_TIM_GET_COUNTER(encoder_htim);
		encoderDirection = __HAL_TIM_IS_TIM_COUNTING_DOWN(encoder_htim);
		uart_helper.printf("\r\nCount=%i\r\n", encoderCount);
		uart_helper.printf("\r\nDirection=%i\r\n", encoderDirection);
		osDelay(500);
	}
}

void setup(UART_HandleTypeDef *main_huart, SPI_HandleTypeDef *main_hspi1,
		TIM_HandleTypeDef *main_htim, TIM_HandleTypeDef *main_htim2,
		TIM_HandleTypeDef *main_encoder_htim1, TIM_HandleTypeDef *main_encoder_htim2)
{

	  huart = main_huart;
	  hspi = main_hspi1;
	  uart_helper.init(huart);

	  socket_client = new SocketClient(hspi, &uart_helper);

	  //****** UART **********
	  osThreadDef(UartTask, StartUARTTask, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(UartTask), NULL);

	  //============ SOCKET CHECKING ==========

	  //****** Check Errors **********
	  osThreadDef(SocketErrorTask, StartSocketStateTask, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(SocketErrorTask), NULL);

	  //****** Check Freezing **********
	  osThreadDef(CheckFreezingTask, StartCheckFreezingTask, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(CheckFreezingTask), NULL);
	  //=======================================

	  //========== ROS ===============

	  //******** SpinOnce ***********
	  ros_helper = new RosHelper();
	  ros_helper->setupRos(&uart_helper, main_htim, main_htim2, main_encoder_htim1, main_encoder_htim2);
	  osThreadDef(RosTask, StartRosTask, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(RosTask), NULL);

	  //**** Wheel1 subscriber ********
	  osThreadDef(setSpeedTask, StartSetSpeedTask, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(setSpeedTask), NULL);

	  //**** Wheel2 subscriber ********
	  osThreadDef(setSpeedTask2, StartSetSpeedTask2, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(setSpeedTask2), NULL);

	  //**** Wheel1 getting encoder info ********
	  osThreadDef(encoderInfoTask, StartEncoderTask, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(encoderInfoTask), NULL);

	  //**** Wheel2 getting encoder info ********
	  osThreadDef(encoderInfoTask2, StartEncoderTask2, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(encoderInfoTask2), NULL);

	  //==============================

	  //****** UART Test ***************
//	  osThreadDef(SecondTask, StartSecondTask, osPriorityNormal, 1, 256);
//	  osThreadCreate(osThread(SecondTask), NULL);

	  //******* Socket Test *************
//	  osThreadDef(SocketSendTask, StartSocketSendTask, osPriorityNormal, 1, 256);
//	  osThreadCreate(osThread(SocketSendTask), NULL);

	  //******* Wheel Speed Test ***********
//	  htim = main_htim;
//	  htim2 = main_htim2;
//
//	  osThreadDef(wheelSpeedTask, StartWheelSpeedTask, osPriorityNormal, 1, 256);
//	  osThreadCreate(osThread(wheelSpeedTask), NULL);

	  //******* Encoder Test ***********
//	  encoder_htim = main_encoder_htim1;
//	  osThreadDef(EncoderTestTask, StartEncoderTestTask, osPriorityNormal, 1, 256);
//	  osThreadCreate(osThread(EncoderTestTask), NULL);

}

