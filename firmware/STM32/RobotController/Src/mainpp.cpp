#include <mainpp.h>
#include <string>
#include "UartHelper.h"
#include "SocketClient.h"
#include <ros.h>
#include <std_msgs/String.h>


UART_HandleTypeDef *huart;
SPI_HandleTypeDef *hspi;
UartHelper uart_helper;
SocketClient socket_client;

ros::NodeHandle nh;

std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);

void setupRos(void)
{
  nh.initNode();
  nh.advertise(chatter);

}


void rosLoop(void)
{
  str_msg.data = "Hello world!";
  chatter.publish(&str_msg);
  nh.spinOnce();
  osDelay(500);
}

void StartRosTask(void const * argument){

	for(;;){
		rosLoop();
	}
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
	  huart = main_huart;
	  hspi = main_hspi1;
	  uart_helper.init(huart);
	  socket_client.init(hspi, &uart_helper);
	  socket_client.socket_connect();

	  setupRos();

	  osThreadDef(SecondTask, StartSecondTask, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(SecondTask), NULL);

	  osThreadDef(UartTask, StartUARTTask, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(UartTask), NULL);

//	  osThreadDef(SocketSendTask, StartSocketSendTask, osPriorityNormal, 1, 256);
//	  osThreadCreate(osThread(SocketSendTask), NULL);

	  osThreadDef(RosTask, StartRosTask, osPriorityNormal, 1, 256);
	  osThreadCreate(osThread(RosTask), NULL);


}

