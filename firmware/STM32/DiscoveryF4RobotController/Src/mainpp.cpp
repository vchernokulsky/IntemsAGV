#include "mainpp.h"
#include "SocketClient.h"
#include "SocketServer.h"
#include "UartHelper.h"
#include "RosHelper.h"

SocketClient socket_client;
UartHelper uart_helper;
RosHelper ros_helper;



static __attribute__ ((used,section(".user_heap_stack"))) uint8_t heap_sram1[32*1024];
uint8_t heap_sram2[32*1024];

void StartSocetClientTask(void *arg);
void StartSocetServerTask(void *arg);
void StartUARTTask(void *arg);
void StartSecondTask(void *arg);
void StartRosTask(void *arg);
void StartSetSpeedTask(void *arg);
void StartSetSpeedTask2(void *arg);
void StartEncoderTask(void *arg);
void StartEncoderTask2(void *arg);

/***** CALL IN 'USER CODE BEGIN(END) 1' *****/
void memory_setup()
{
	  HeapRegion_t xHeapRegions[] =
	  {
	      {  heap_sram1, sizeof(heap_sram1) },
		  {  heap_sram2, sizeof(heap_sram2) },
	      { NULL, 0 }
	  };
	  vPortDefineHeapRegions( xHeapRegions );
}
/********************************************/

/*************** CALL IN 'USER CODE BEGIN(END) 5' ***************/
void threds_setup(UART_HandleTypeDef *main_huart, TIM_HandleTypeDef *main_htim,  TIM_HandleTypeDef *main_htim2, TIM_HandleTypeDef *encoder_htim, TIM_HandleTypeDef *encoder_htim2)
{

	uart_helper.init(main_huart);
	socket_client.init(10888, "192.168.55.10", 11411);
	ros_helper.setupRos(&uart_helper, main_htim, main_htim2, encoder_htim, encoder_htim2);


	//****** Client Task **********
	sys_thread_new("client_thread", StartSocetClientTask, 0, DEFAULT_THREAD_STACKSIZE * 2, osPriorityNormal);
	sys_thread_new("server_thread", StartSocetServerTask, 0, DEFAULT_THREAD_STACKSIZE * 2, osPriorityNormal);
	sys_thread_new("uart_thread", StartUARTTask, 0, DEFAULT_THREAD_STACKSIZE * 2, osPriorityNormal);
	sys_thread_new("uart_test_thread", StartSecondTask, 0, 256, osPriorityNormal);
	sys_thread_new("ros_thread", StartRosTask, 0, DEFAULT_THREAD_STACKSIZE * 2, osPriorityNormal);
	sys_thread_new("wheel1_thread", StartSetSpeedTask, 0, DEFAULT_THREAD_STACKSIZE * 2, osPriorityNormal);
	sys_thread_new("wheel2_thread", StartSetSpeedTask2, 0, DEFAULT_THREAD_STACKSIZE * 2, osPriorityNormal);
	sys_thread_new("encoder1_thread", StartEncoderTask, 0, DEFAULT_THREAD_STACKSIZE * 2, osPriorityNormal);
	sys_thread_new("encoder2_thread", StartEncoderTask2, 0, DEFAULT_THREAD_STACKSIZE * 2, osPriorityNormal);

}
/***************************************************************************/

void StartSocetClientTask(void *arg)
{

	socket_client.SocketClientTask();
}

void StartSocetServerTask(void *arg)
{
	SocketServer server(11511);
	server.SocketServerTask();
}
void StartUARTTask(void *arg)
{
	uart_helper.UARTTask();
}

void StartSecondTask(void *arg)
{
  for(;;)
  {
	  uint8_t buff[] = "!!!!!!!!!!!!!!!!\n\0";
	  uart_helper.printf(buff);
	  osDelay(500);
  }
}
void StartRosTask(void *arg)
{
	ros_helper.RosTask();
}

void StartSetSpeedTask(void *arg){
	ros_helper.setSpeedTask();
}
void StartSetSpeedTask2(void *arg){
	ros_helper.setSpeedTask2();
}

void StartEncoderTask(void *arg){
	ros_helper.encoderTask();
}

void StartEncoderTask2(void *arg){
	ros_helper.encoderTask2();
}

