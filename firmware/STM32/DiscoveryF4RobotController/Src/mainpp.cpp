#include "mainpp.h"

#include "SocketClient.h"
#include "SocketServer.h"
#include "RosHelper.h"
#include "System_config.h"


SocketClient socket_client;
RosHelper ros_helper;



static __attribute__ ((used,section(".user_heap_stack"))) uint8_t heap_sram1[32*1024];
uint8_t heap_sram2[32*1024];

void StartSocetClientTask(void *arg);
void StartSocetServerTask(void *arg);
void StartRosTask(void *arg);
void StartSetSpeedTask(void *arg);
void StartSetSpeedTask2(void *arg);
void StartEncoderTask(void *arg);
void StartEncoderTask2(void *arg);
void StartCmdvelTimeoutRask(void *arg);

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

/*************** CALL IN 'USER CODE BEGIN(END) 5' *************
 *
 * replace MX_LWIP_Init() with LWIP_Init() from "lwip_init.h" in StartDefaultTask
 * to set ip configuration from "User_config.h"*
 *
 * */
void threds_setup(TIM_HandleTypeDef *main_htim,  TIM_HandleTypeDef *main_htim2, TIM_HandleTypeDef *encoder_htim, TIM_HandleTypeDef *encoder_htim2)
{
;

	socket_client.init(10888, "192.168.2.150", 11411);
	ros_helper.setupRos(main_htim, main_htim2, encoder_htim, encoder_htim2);


	//****** Client Task **********
	sys_thread_new("client_thread", StartSocetClientTask, 0, DEFAULT_THREAD_STACKSIZE * 2, osPriorityNormal);
//	sys_thread_new("server_thread", StartSocetServerTask, 0, DEFAULT_THREAD_STACKSIZE * 2, osPriorityNormal);
	sys_thread_new("ros_thread", StartRosTask, 0, 256, osPriorityNormal);
	sys_thread_new("wheel1_thread", StartSetSpeedTask, 0, 256, osPriorityNormal);
	sys_thread_new("wheel2_thread", StartSetSpeedTask2, 0, 256, osPriorityNormal);
	sys_thread_new("encoder1_thread", StartEncoderTask, 0, 256, osPriorityNormal);
	sys_thread_new("encoder2_thread", StartEncoderTask2, 0, 256, osPriorityNormal);
	sys_thread_new("cmdvel_timeout_thread", StartCmdvelTimeoutRask, 0, 128, osPriorityNormal);

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

void StartRosTask(void *arg)
{
	ros_helper.RosTask();
}

void StartSetSpeedTask(void *arg)
{
	ros_helper.setSpeedTask();
}
void StartSetSpeedTask2(void *arg)
{
	ros_helper.setSpeedTask2();
}

void StartEncoderTask(void *arg)
{
	ros_helper.encoderTask();
}

void StartEncoderTask2(void *arg)
{
	ros_helper.encoderTask2();
}

void StartCmdvelTimeoutRask(void *arg)
{
	ros_helper.cmdvelTimeoutTask();
}


